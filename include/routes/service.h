#ifndef _ROUTES_SERVICE_H_
#define _ROUTES_SERVICE_H_

struct _HttpReceive;
struct _HttpRequest;

// GET /api/push
extern void push_main_handler (
	const struct _HttpReceive *http_receive,
	const struct _HttpRequest *request
);

// GET /api/push/version
extern void push_version_handler (
	const struct _HttpReceive *http_receive,
	const struct _HttpRequest *request
);

// GET *
extern void push_catch_all_handler (
	const struct _HttpReceive *http_receive,
	const struct _HttpRequest *request
);

#endif