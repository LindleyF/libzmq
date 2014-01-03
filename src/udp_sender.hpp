/*
    Copyright (c) 2012 Martin Lucina <martin@lucina.net>

    This file is part of Crossroads I/O.

    Crossroads I/O is free software; you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    Crossroads I/O is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __ZMQ_UDP_SENDER_HPP_INCLUDED__
#define __ZMQ_UDP_SENDER_HPP_INCLUDED__

#include "platform.hpp"

#include "stdint.hpp"
#include "io_object.hpp"
#include "i_engine.hpp"
#include "options.hpp"
#include "tcp_address.hpp"
#include "v1_encoder.hpp"

namespace zmq
{

    class io_thread_t;
    class session_base_t;

    class udp_sender_t : public io_object_t, public i_engine
    {

    public:

        udp_sender_t (zmq::io_thread_t *parent_, const options_t &options_);
        ~udp_sender_t ();

        int init (const char *address_);

        //  i_engine interface implementation.
        void plug (zmq::io_thread_t *io_thread_,
            zmq::session_base_t *session_);
        void terminate ();
        void restart_input ();
        void restart_output ();
	void zap_msg_available() {}

        //  i_poll_events interface implementation.
        void in_event (fd_t fd_);
        void out_event (fd_t fd_);
        void timer_event (int token);

    private:

        //  TX and RX timeout timer ID's.
        enum {tx_timer_id = 0xa0, rx_timer_id = 0xa1};

        void unplug ();

        //  Underlying UDP socket.
        fd_t socket;
        handle_t socket_handle;

        //  Socket address.
        tcp_address_t address;

        //  Socket options.
        options_t options;

        //  Encoder for this socket.
        v1_encoder_t encoder;

        //  Associated session.
        zmq::session_base_t *session;

        //  UDP packet header size.
        static const size_t udp_header_size = 6;

        //  Packet buffer for outgoing packets.
        unsigned char data [pgm_max_tpdu];

        //  Sequence number for outgoing packets.
        uint32_t seq_no;

	bool has_rx_timer;
	bool has_tx_timer;

        udp_sender_t (const udp_sender_t&);
        const udp_sender_t &operator = (const udp_sender_t&);
    };

}

#endif
