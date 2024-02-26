#include "websocket.h"

#include <stdio.h>
#include <libwebsockets.h>

/* Global variables ***********************************************************/

static struct lws_context* context;

/* Function prototypes ********************************************************/

static int ws_cb(struct lws* wsi, enum lws_callback_reasons reason,
		void* user, void* in, size_t len);

/* Callbacks ******************************************************************/

static int ws_cb(struct lws* wsi, enum lws_callback_reasons reason,
		void* user, void* in, size_t len)
{
	struct my_conn_data* conn_data = (struct my_conn_data*)user;
	char* rsp = "Hello, client!";
	(void)conn_data;
	switch (reason) {
		case LWS_CALLBACK_ESTABLISHED:
			printf("Connection established\n");
			break;
		case LWS_CALLBACK_RECEIVE:
			printf("Received data (len=%d): %s\n", (int)len, (char*)in);
			usleep(5000000);
			lws_write(wsi, (unsigned char*)rsp, strlen(rsp), LWS_WRITE_TEXT);
			break;
		case LWS_CALLBACK_SERVER_WRITEABLE:
			printf("Sending data...\n");
			lws_write(wsi, (unsigned char*)rsp, strlen(rsp), LWS_WRITE_TEXT);
			break;
		case LWS_CALLBACK_CLOSED:
			printf("Connection closed\n");
			break;
		default:
			break;
	}
	return 0;
}

/* Function definitions *******************************************************/
/* Public functions ***********************************************************/

void websocket_init()
{
	static struct lws_protocols protocols[] = {
		{
			.name                  = "example-protocol", /* Protocol name*/
			.callback              = ws_cb,              /* Protocol callback */
			.per_session_data_size = 0,                  /* Protocol callback 'userdata' size */
			.rx_buffer_size        = 0,                  /* Receve buffer size (0 = no restriction) */
			.id                    = 0,                  /* Protocol Id (version) (optional) */
			.user                  = NULL,               /* 'User data' ptr, to access in 'protocol callback */
			.tx_packet_size        = 0                   /* Transmission buffer size restriction (0 = no restriction) */
		},
		LWS_PROTOCOL_LIST_TERM /* terminator */
	};

	struct lws_context_creation_info info;
	memset(&info, 0, sizeof(info));
	info.port = 8080;
	info.protocols = protocols;

	context = lws_create_context(&info);
	if (!context) {
		fprintf(stderr, "Error creating context\n");
		return;
	}
	printf("Websocket server started\n");
}

void websocket_send()
{
	printf("Send!\n");
}

void websocket_fire()
{
	lws_service(context, 50);
}

void websocket_close()
{
	lws_context_destroy(context);
}