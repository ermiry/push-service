#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <time.h>
#include <signal.h>

#include <cerver/cerver.h>
#include <cerver/version.h>

#include <cerver/http/http.h>
#include <cerver/http/route.h>

#include <cerver/utils/log.h>
#include <cerver/utils/utils.h>

#include "service.h"
#include "version.h"

#include "routes/service.h"
#include "routes/transactions.h"

static Cerver *push_service = NULL;

void end (int dummy) {

	if (push_service) {
		cerver_stats_print (push_service, false, false);
		(void) printf ("\nHTTP Cerver stats:\n");
		http_cerver_all_stats_print ((HttpCerver *) push_service->cerver_data);
		(void) printf ("\n");
		cerver_teardown (push_service);
	}

	service_end ();

	cerver_end ();

	#ifdef SERVICE_DEBUG
	(void) printf ("\n\nDone!\n\n");
	#endif

	exit (0);

}

static void push_set_service_routes (HttpCerver *http_cerver) {

	/*** top level route ***/

	// GET /api/push
	HttpRoute *main_route = http_route_create (REQUEST_METHOD_GET, "api/push", push_main_handler);
	http_cerver_route_register (http_cerver, main_route);

	// HEAD /api/push
	http_route_set_handler (main_route, REQUEST_METHOD_HEAD, push_main_handler);

	/*** service routes ***/

	// GET /api/push/version
	HttpRoute *version_route = http_route_create (REQUEST_METHOD_GET, "version", push_version_handler);
	http_route_child_add (main_route, version_route);

	// HEAD /api/push/version
	http_route_set_handler (version_route, REQUEST_METHOD_HEAD, push_version_handler);

	// POST /api/push/transaction
	HttpRoute *create_trans_route = http_route_create (REQUEST_METHOD_POST, "transaction", service_transaction_create_handler);
	http_route_child_add (main_route, create_trans_route);

}

static void start (void) {

	push_service = cerver_create (
		CERVER_TYPE_WEB,
		"push-service",
		(u16) PORT,
		PROTOCOL_TCP,
		false,
		CERVER_CONNECTION_QUEUE
	);

	if (push_service) {
		/*** cerver configuration ***/
		cerver_set_alias (push_service, "push");

		cerver_set_receive_buffer_size (push_service, CERVER_RECEIVE_BUFFER_SIZE);
		cerver_set_thpool_n_threads (push_service, CERVER_TH_THREADS);
		cerver_set_handler_type (push_service, CERVER_HANDLER_TYPE_THREADS);

		cerver_set_reusable_address_flags (push_service, true);

		/*** web cerver configuration ***/
		HttpCerver *http_cerver = (HttpCerver *) push_service->cerver_data;

		push_set_service_routes (http_cerver);

		// add a catch all route
		http_cerver_set_catch_all_route (http_cerver, push_catch_all_handler);

		// admin configuration
		http_cerver_enable_admin_routes (http_cerver, true);

		if (cerver_start (push_service)) {
			cerver_log_error (
				"Failed to start %s!",
				push_service->info->name
			);

			cerver_delete (push_service);
		}
	}

	else {
		cerver_log_error ("Failed to create cerver!");

		cerver_delete (push_service);
	}

}

int main (int argc, char const **argv) {

	srand ((unsigned int) time (NULL));

	// register to the quit signal
	(void) signal (SIGINT, end);
	(void) signal (SIGTERM, end);

	// to prevent SIGPIPE when writting to socket
	(void) signal (SIGPIPE, SIG_IGN);

	cerver_init ();

	cerver_version_print_full ();

	service_version_print_full ();

	if (!service_init ()) {
		start ();
	}

	else {
		cerver_log_error ("Failed to init service!");
	}

	service_end ();

	cerver_end ();

	return 0;

}
