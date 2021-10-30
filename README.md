# Test Push Service

### Development
```
sudo docker run \
  -it \
  --name push --rm \
  -p 5000:5000 --net worker \
  -v /home/ermiry/Documents/ermiry/Projects/push-service:/home/push \
  -e RUNTIME=development \
  -e PORT=5000 \
  -e CERVER_RECEIVE_BUFFER_SIZE=4096 -e CERVER_TH_THREADS=4 \
  -e CERVER_CONNECTION_QUEUE=4 \
  -e MONGO_APP_NAME=push -e MONGO_DB=worker \
  -e MONGO_URI=mongodb://push:password@mongo:27017/worker \
  -e CONNECT_TO_REDIS=true -e REDIS_HOSTNAME=redis \
  ermiry/push-service:development /bin/bash
```

## Routes

#### GET /api/push
**Access:** Public \
**Description:** Push service top level route \
**Returns:**
  - 200 on success

#### GET /api/push/version
**Access:** Public \
**Description:** Returns push service current version \
**Returns:**
  - 200 on success

#### POST /api/push/transaction
**Access:** Public \
**Description:** A user has requested to create a new transaction \
**Returns:**
  - 200 on success creating transaction
  - 400 on failed to create new transaction
  - 401 on failed auth
  - 500 on server error
