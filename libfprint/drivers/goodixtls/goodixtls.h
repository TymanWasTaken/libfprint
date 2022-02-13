// Goodix Tls driver for libfprint

// Copyright (C) 2021 Alexander Meiler <alex.meiler@protonmail.com>
// Copyright (C) 2021 Matthieu CHARETTE <matthieu.charette@gmail.com>

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#pragma once

#include <glib.h>

#include <stdatomic.h>

#define GOODIX_TLS_SERVER_PORT 4433

// static const guint8 goodix_511_psk_0[64] = {0};
static const guint8 goodix_511_psk_0[] = {
    0x66, 0x68, 0x7a, 0xad, 0xf8, 0x62, 0xbd, 0x77, 0x6c, 0x8f, 0xc1, 
    0x8b, 0x8e, 0x9f, 0x8e, 0x20, 0x08, 0x97, 0x14, 0x85, 0x6e, 0xe2, 
    0x33, 0xb3, 0x90, 0x2a, 0x59, 0x1d, 0x0d, 0x5f, 0x29, 0x25};

struct _GoodixTlsServer;

typedef void (*GoodixTlsServerSendCallback)(struct _GoodixTlsServer* self,
                                            guint8* data, guint16 length);

typedef void (*GoodixTlsServerConnectionCallback)(struct _GoodixTlsServer* self,
                                                  GError* error,
                                                  gpointer user_data);

typedef void (*GoodixTlsServerDecodedCallback)(struct _GoodixTlsServer* self,
                                               guint8* data, gsize length,
                                               GError* error);

typedef struct _GoodixTlsServer {
    // This callback should be called when a TLS packet must be send to the
    // device
    // GoodixTlsServerSendCallback send_callback;

    // This callback should be called when the connection is established. The
    // error should be NULL. It can also be called when the connection fail. In
    // this case, the error should not be NULL.
    GoodixTlsServerConnectionCallback connection_callback;

    // This callback should be called when a TLS packet is decoded. The error
    // should be NULL.
    // It can also be called when the server fail to decode a packet. In this
    // case, the error should not be NULL.
    // GoodixTlsServerDecodedCallback decoded_callback;

    // Put what you need here.
    gpointer user_data; // Passed to all callbacks
    SSL_CTX* ssl_ctx;
    int sock_fd;
    SSL* ssl_layer;
    // SSL* cli_ssl_layer;
    int client_fd;
    pthread_t serve_thread;
} GoodixTlsServer;

// This is called only once to init the TLS server.
// Return TRUE on success, FALSE otherwise and error should be set.
gboolean goodix_tls_server_init(GoodixTlsServer* self, GError** error);

gboolean goodix_tls_init_cli(GoodixTlsServer* self, GError** err);

// This can be called multiple times. It is called when the device send a TLS
// packet.
int goodix_tls_server_receive(GoodixTlsServer* self, guint8* data,
                              guint32 length, GError** error);

int goodix_tls_client_send(GoodixTlsServer* self, guint8* data, guint16 length);

int goodix_tls_client_recv(GoodixTlsServer* self, guint8* data, guint16 length);

// This is called only once to deinit the TLS server.
// Return TRUE on success, FALSE otherwise and error should be set.
gboolean goodix_tls_server_deinit(GoodixTlsServer* self, GError** error);
