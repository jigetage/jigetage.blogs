```shell
$ JOBMANAGER_CONTAINER=$(docker ps --filter name=jobmanager --format={{.ID}})
$ docker cp path/to/jar "$JOBMANAGER_CONTAINER":/job.jar
$ docker exec -t -i "$JOBMANAGER_CONTAINER" flink run /job.jar
```

