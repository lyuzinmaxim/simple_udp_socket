# simple_udp_socket
Simple UDP socket variations

To compile client side (send bytes)

``` 
gcc client.c thpool.c -D THPOOL_DEBUG -pthread -o client
```

To compile server side (receive bytes)

```
gcc server.c -o server
```
