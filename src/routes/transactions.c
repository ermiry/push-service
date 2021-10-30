#include <stdlib.h>

#include <cerver/http/http.h>
#include <cerver/http/response.h>

#include "errors.h"
#include "service.h"

#include "controllers/transactions.h"

// POST /api/push/transaction
void service_transaction_create_handler (
	const HttpReceive *http_receive,
	const HttpRequest *request
) {

	ServiceError error = service_trans_create (request->body);

	switch (error) {
		case SERVICE_ERROR_NONE: {
			// return success to user
			(void) http_response_send (
				trans_created_success,
				http_receive
			);
		} break;

		default: {
			service_error_send_response (error, http_receive);
		} break;
	}

}
