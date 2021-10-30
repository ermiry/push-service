#ifndef _ROUTES_TRANSACTIONS_H_
#define _ROUTES_TRANSACTIONS_H_

struct _HttpReceive;
struct _HttpResponse;

// POST /api/push/transaction
// a user has requested to create a new transaction
extern void service_transaction_create_handler (
	const struct _HttpReceive *http_receive,
	const struct _HttpRequest *request
);

#endif