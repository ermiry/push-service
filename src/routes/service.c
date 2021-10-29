#include <stdlib.h>

#include <cerver/http/http.h>
#include <cerver/http/request.h>
#include <cerver/http/response.h>

#include "controllers/service.h"

// GET /api/push
void push_main_handler (
	const HttpReceive *http_receive,
	const HttpRequest *request
) {

	(void) http_response_send (push_works, http_receive);

}

// GET /api/push/version
void push_version_handler (
	const HttpReceive *http_receive,
	const HttpRequest *request
) {

	(void) http_response_send (current_version, http_receive);

}

// GET *
void push_catch_all_handler (
	const HttpReceive *http_receive,
	const HttpRequest *request
) {

	(void) http_response_send (not_found_error, http_receive);

}
