//----------------------------------------------------------------------------
//
//  This file is part of seq32.
//
//  seq32 is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  seq32 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with seq32; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//-----------------------------------------------------------------------------

#include <cctype>
#include <csignal>
#include <cerrno>
#include <cstring>


#include "perform.h"

#include "mainterm.h"
#include "midifile.h"

extern bool global_is_running;
extern bool global_is_modified;



mainterm::mainterm(perform *a_p):
    m_mainperf(a_p),
    //m_app(app),
    //m_menu_mode(false),
    m_perf_edit(NULL)
    //m_options(NULL),
    //m_closing_windows(false)
{
    /* register for notification */
    m_mainperf->m_notify.push_back( this );

    /* main window */
    //update_window_title();
}

///////////////////////////////////////////////////////////////////////////////





void mainterm::new_file()
{
    /* reset everything to default */
    if(m_mainperf->clear_all())
    {
        m_perf_edit->clear_tempo_list();
        m_perf_edit->update_start_BPM(c_bpm);
        m_perf_edit->set_bp_measure(4);
        m_perf_edit->set_bw(4);
        m_perf_edit->set_xpose(0);
        m_mainperf->set_playlist_mode(false);

	//todo(sjh): what would we need to reset in ncurses?
        //m_main_wid->reset();
        
        //todo(sjh): what would we need to set in ncurses?
        //m_entry_notes->set_text( * m_mainperf->get_screen_set_notepad(
        //                             m_mainperf->get_screenset() ));

        global_filename = "";
        
        //update_window_title();
        //update_window_xpm();
        
        global_is_modified = false;
    }
    else
    {
        //new_open_error_dialog();
        printf("Unspecified error! m_mainperf->clear_all() returned false\n");
    }
}






bool mainterm::open_file(const Glib::ustring& fn)
{
    /* reset everything to default */
    if(m_mainperf->clear_all())
    {
        m_perf_edit->clear_tempo_list();
        m_perf_edit->set_xpose(0);

        midifile f(fn);
        //bool result = f.parse(m_mainperf, this, 0);
        bool result = f.parse(m_mainperf, NULL, 0);

        global_is_modified = !result; /* this means good file = NOT modified and bad = modified?? */

        if (!result)
        {
            //Gtk::MessageDialog errdialog(*this,
            //                             "Error reading file: " + fn, false,
            //                             Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
            //errdialog.run();
            printf("Error reading file: %s\n", std::string(fn).c_str());
            global_filename = "";
            new_file();
            return false;
        }

        last_used_dir = fn.substr(0, fn.rfind("/") + 1);
        global_filename = fn;
        
        if(!m_mainperf->get_playlist_mode())           /* don't list files from playlist */
        {
            m_mainperf->add_recent_file(fn);           /* from Oli Kester's Kepler34/Sequencer 64       */
            //update_recent_files_menu();
        }
        
        //update_window_title();
        //update_window_xpm();

	//todo(sjh): what would we need to reset in ncurses?
        //m_main_wid->reset();
        
        //todo(sjh): what would we need to set in ncurses?
        //m_entry_notes->set_text(*m_mainperf->get_screen_set_notepad(
        //                            m_mainperf->get_screenset()));
    }
    else
    {
        //new_open_error_dialog();
        printf("Unspecified error! m_mainperf->clear_all() returned false\n");
        return false;
    }

    return true;
}











/*
 * move through the playlist (jmp is 0 on start and 1 if right arrow, -1 for left arrow)
 */
bool mainterm::playlist_jump(int jmp, bool a_verify)
{
    if(global_is_running)                       // don't allow jump if running
        return false;
    
    bool result = false;
    if(a_verify)                                // we will run through all the files
    {
        m_mainperf->set_playlist_index(0);      // start at zero
        jmp = 0;                                // to get the first one
    }

    while(1)
    {
        if(m_mainperf->set_playlist_index(m_mainperf->get_playlist_index() + jmp))
        {
            if(Glib::file_test(m_mainperf->get_playlist_current_file(), Glib::FILE_TEST_EXISTS))
            {
                if(open_file(m_mainperf->get_playlist_current_file()))
                {
                    if(a_verify)    // verify whole playlist
                    {
                        jmp = 1;    // after the first one set to 1 for jump
                        continue;   // keep going till the end of list
                    }
                    result = true;
                    break;
                }
                else
                {
                    Glib::ustring message = "Playlist file open error\n";
                    message += m_mainperf->get_playlist_current_file();
                    m_mainperf->error_message_gtk(message);
                    m_mainperf->set_playlist_mode(false);    // abandon ship
                    result = false;
                    break;  
                }
            }
            else
            {
                Glib::ustring message = "Midi playlist file does not exist\n";
                message += m_mainperf->get_playlist_current_file();
                m_mainperf->error_message_gtk(message);
                m_mainperf->set_playlist_mode(false);        // abandon ship
                result = false;
                break;  
            }
        }
        else                                                // end of file list
        {
            result = true;   // means we got to the end or beginning, without error
            break;
        }
    }
    
    if(!result)                                             // if errors occured above
    {
        //update_window_title();
        //update_window_xpm();
    }
    return result;
}





void
mainterm::playlist_verify()
{
    bool result = false;
    
    result = playlist_jump(PLAYLIST_ZERO,true); // true is verify mode
    
    if(result)                                  // everything loaded
    {
        m_mainperf->set_playlist_index(0);      // set to start
        playlist_jump(PLAYLIST_ZERO);                       // load the first file
        printf("Playlist verification was successful!\n");
    }
    else                                        // verify failed somewhere
    {
        new_file();                             // clear and start clean
    }
}
