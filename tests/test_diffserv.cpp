/*
    Copyright (c) 2007-2013 Contributors as noted in the AUTHORS file

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "testutil.hpp"

int main (void)
{
    int rc;
    int tos = 0x28;
    int o_tos;
    size_t tos_size = sizeof(tos);

    setup_test_environment();
    void *ctx = zmq_ctx_new ();
    assert (ctx);

    void *sb = zmq_socket (ctx, ZMQ_PAIR);
    assert (sb);
    rc = zmq_setsockopt (sb, ZMQ_TOS, &tos, tos_size);
    assert (rc == 0);
    rc = zmq_bind (sb, "tcp://127.0.0.1:5560");
    assert (rc == 0);
    rc = zmq_getsockopt (sb, ZMQ_TOS, &o_tos, &tos_size);
    assert (rc == 0);
    assert (o_tos == tos);

    void *sc = zmq_socket (ctx, ZMQ_PAIR);
    assert (sc);
    tos = 0x58;
    rc = zmq_setsockopt (sc, ZMQ_TOS, &tos, tos_size);
    assert (rc == 0);
    rc = zmq_connect (sc, "tcp://127.0.0.1:5560");
    assert (rc == 0);
    rc = zmq_getsockopt (sc, ZMQ_TOS, &o_tos, &tos_size);
    assert (rc == 0);
    assert (o_tos == tos);

    // Wireshark can be used to verify that the server socket is
    // using DSCP 0x28 in packets to the client while the client
    // is using 0x58 in packets to the server.
    bounce (sb, sc);

    rc = zmq_close (sc);
    assert (rc == 0);

    rc = zmq_close (sb);
    assert (rc == 0);

    rc = zmq_ctx_term (ctx);
    assert (rc == 0);

    return 0 ;

}
