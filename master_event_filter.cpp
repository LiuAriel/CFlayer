/******************************************************************************
	CCAV:  Media play library based on Qt(UI),c/c++11 and FFmpeg
	Copyright (C) 2015-2016 Juno <junowendy@gmail.com>
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/


#include <CCAV/master_event_filter.h>
#include <QApplication>
#include <QEvent>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMouseEvent>
#include <CCAV/master_player.h>
#include <CCAV/output_audio.h>
#include <functional>
namespace CCAV {

EventFilter::EventFilter(CCPlayer *parent) :
    QObject(parent),player(parent)
{
}

bool EventFilter::eventFilter(QObject *watched, QEvent *event)
{
  
    Q_UNUSED(watched);
   
    QEvent::Type type = event->type();
    switch (type) {
    case QEvent::MouseButtonPress:
        qDebug("EventFilter: Mouse press");
        static_cast<QMouseEvent*>(event)->button();
        return false;
        break;
    case QEvent::KeyPress: {
       
        int key = static_cast<QKeyEvent*>(event)->key();
        switch (key) {
        case Qt::Key_N: 
			player->func_play_next_frame = std::bind(&CCPlayer::play_next_frame, player);
			player->func_play_next_frame();
            break;
        case Qt::Key_P:
			player->func_play = std::bind(&CCPlayer::plays, player);
			player->func_play();           
            break;
        case Qt::Key_S:
			player->func_stop_data = std::bind(&CCPlayer::stop_datas, player);
			player->func_stop_data();
            break;
		case Qt::Key_Space:
		{
			qDebug("is_paused = %d", player->is_paused());
			player->func_pauses = std::bind(&CCPlayer::pauses, player, std::placeholders::_1);
			player->func_pauses(!player->is_paused());
		}
            break;
        case Qt::Key_F: { 
            QWidget *w = qApp->activeWindow();
            if (!w)
                return false;
            if (w->isFullScreen())
                w->showNormal();
            else
                w->showFullScreen();
        }
            break;
        case Qt::Key_Up:
            if (player->audio) {
                double v = player->audio->volume();
                if (v > 0.5)
                    v += 0.1;
                else if (v > 0.1)
                    v += 0.05;
                else
                    v += 0.025;
                player->audio->set_volume(v);
                qDebug("vol = %.3f", player->audio->volume());
            }
            break;
        case Qt::Key_Down:
            if (player->audio) {
                double v = player->audio->volume();
                if (v > 0.5)
                    v -= 0.1;
                else if (v > 0.1)
                    v -= 0.05;
                else
                    v -= 0.025;
                player->audio->set_volume(v);
                qDebug("vol = %.3f", player->audio->volume());
            }
            break;
        case Qt::Key_O: {
            QString file = QFileDialog::getOpenFileName(0, tr("Open a video"));
			
            if (!file.isEmpty())
				player->func_open_file = std::bind(&CCPlayer::play, player, std::placeholders::_1);
			    player->func_open_file(file);
        }
            break;
        case Qt::Key_Left:
            qDebug("<-");
			player->func_seek_backward = std::bind(&CCPlayer::seek_backward, player);
			player->func_seek_backward();
            break;
        case Qt::Key_Right:
            qDebug("->");
			player->func_seek_forward = std::bind(&CCPlayer::seek_forward, player);
			player->func_seek_forward();
            break;
        case Qt::Key_M:
            if (player->audio) {
                player->audio->set_mute(!player->audio->is_mute());
            }
            break;
        case Qt::Key_T: {
            QWidget *w = qApp->activeWindow();
            if (!w)
                return false;
            Qt::WindowFlags wf = w->windowFlags();
            if (wf & Qt::WindowStaysOnTopHint) {
                qDebug("Window not stays on top");
                w->setWindowFlags(wf & ~Qt::WindowStaysOnTopHint);
            } else {
                qDebug("Window stays on top");
                w->setWindowFlags(wf | Qt::WindowStaysOnTopHint);
            }
           
            w->show();
        }
            break;
        default:
            return false;
        }
        break;
    }
    default:
        return false;
    }
    return true; 
}

} //namespace CCAV
