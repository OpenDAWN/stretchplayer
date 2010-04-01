/*
 * Copyright(c) 2009 by Gabriel M. Beddingfield <gabriel@teuton.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <jack/jack.h>
#include <memory>
#include <QString>
#include <QMutex>
#include <vector>

namespace RubberBand
{
    class RubberBandStretcher;
}

namespace StretchPlayer
{

class Engine
{
public:
    Engine();
    ~Engine();

    void load_song(const QString& filename);
    void play();
    void stop();
    float get_position(); // in seconds
    float get_length();   // in seconds
    void locate(double secs);
    float get_stretch() {
	return _stretch;
    }
    void set_stretch(float str) {
	if(str > 0.5 && str < 2.0) {
	    _stretch = str;
	}
    }
    int get_pitch() {
	return _pitch;
    }
    void set_pitch(int pit) {
	if(pit < -12) {
	    _pitch = -12;
	} else if (pit > 12) {
	    _pitch = 12;
	} else {
	    _pitch = pit;
	}
    }

private:
    static int static_jack_callback(jack_nframes_t nframes, void* arg) {
	Engine *e = static_cast<Engine*>(arg);
	return e->jack_callback(nframes);
    }

    int jack_callback(jack_nframes_t nframes);

    void _zero_buffers(jack_nframes_t nframes);
    void _process_playing(jack_nframes_t nframes);

    jack_client_t* _jack_client;
    jack_port_t *_port_left, *_port_right;

    bool _playing;
    QMutex _audio_lock;
    std::vector<float> _left;
    std::vector<float> _right;
    unsigned long _position;
    float _sample_rate;
    float _stretch;
    int _pitch;
    std::auto_ptr<RubberBand::RubberBandStretcher> _stretcher;

}; // Engine

} // namespace StretchPlayer

#endif // ENGINE_HPP
