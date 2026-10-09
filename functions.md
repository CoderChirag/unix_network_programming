# Socket Function

`int socket(int addr_family, int type, int protocol)`:
- fundamental system call in C that creates an endpoint for network communication.
- very first function you call in almost any network program.
- returns a socket file descriptor
- At this stage, the socket is just an empty shell. It exists, but it has no assigned IP address, no port number, and is not connected to anything.

**Lib:** `<sys/socket.h>`

**Parameters**
1. **domain (Address Family)**: This defines the format of the addresses the socket will use.
    - `AF_INET`: IPv4 network protocols (e.g., 192.168.1.5).
    - `AF_INET6`: IPv6 network protocols.
    - `AF_UNIX` (or `AF_LOCAL`): Local communication between processes on the same machine using file paths.

2. **type (Socket Type)**: This dictates the semantics of communication about how the data will actually be transmitted.
    - `SOCK_STREAM`: Provides sequenced, reliable, two-way, connection-based byte streams. In the `AF_INET` domain, this means TCP.
    - `SOCK_DGRAM`: Supports datagrams (connectionless, unreliable messages of a fixed maximum length). In the `AF_INET` domain, this means UDP.
    - `SOCK_RAW`: Provides access to internal network protocols and interfaces, often used for custom packet crafting (requires root privileges).

3. **protocol**: This specifies a particular protocol to be used with the socket. Usually, there is only one protocol that supports a specific socket type within a given family, so this is almost always set to `0` to let the OS choose the default (e.g., `0` for `SOCK_STREAM` automatically selects TCP).

# Socket Address Structure

**Lib:** `<netinet/in.h>`

## IPv4
```c
struct in_addr {
  in_addr_t s_addr; /* 32-bit IPv4 address */
                    /* network byte ordered */
}

struct sockaddr_in {
  uint8_t        sin_len;      /* length or structure (16) */
  sa_family_t   sin_family;   /* AF_INET */
  in_port_t      sin_port;     /* 16-bit TCP or UDP port number */
                               /* network-byte ordered */
  struct in_addr sin_addr;     /* 32-bit IPv4 address */
                               /* network byte ordered */
  char           sin_zero[8];  /* unused */
}
```

## IPv6
```c
struct in6_addr {
  uint8_t s6_addr[16]; /* 128-bit IPv6 addr */
                       /* network byte ordered */
}

#define SIN6_LEN  /* required for compile-time tests */

struct sockaddr_in6 {
  uint8_t         sin6_len;       /* length of this struct (28) */
  sa_family_t     sin6_family;    /* AF_INET6 */
  in_port_t       sin6_port;      /* transport layer port# */
                                  /* network byte ordered */
  uint32_t        sin6_flowinfo;  /* flow information, undefined */
  struct in6_addr sin6_addr;      /* IPv6 address */
                                  /* network byte ordered */
  uint32_t        sin6_scope_id;  /* set of interfaces for a scope */
}
```

# Socket Address Structure Functions

**Lib:** `<sys/socket.h>`

| Function | Category | Desc |
| - | - | -
| `int bind(int sockfd, const struct sockaddr *srv_addr, socklen_t addrlen)` | passes `sockaddr *` from process -> kernel | Binds process to socket (server side function) |
| `int connect(int sockfd, const struct sockaddr *srv_addr, socklen_t addrlen)` | passes `sockaddr *` from process -> kernel | Connects to a socket (client side function) |
| `ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)` | passes `sockaddr *` from process -> kernel | Transmits data to dest (for connectionless protocol like udp) |
| `ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags)` | passes `sockaddr *` from process -> kernel | |
| `int accept(int sockfd, struct sockaddr *client_addr, socklen_t *addrlen)` | passes `sockaddr *` from kernel -> process | Accepts incoming connection. `socklen_t *addrlen` is a **"value-result"** argument |
| `ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)` | passes `sockaddr *` from kernel -> process | Recieves data from socket (for connectionless protocol like udp) |
| `ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags)` | passes `sockaddr *` from kernel -> process | |
| `int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen)` |  passes `sockaddr *` from kernel -> process | |
| `int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen)` | passes `sockaddr *` from kernel -> process | |

# Byte Ordering Functions

| Function | Desc |
| -- | -- |
| `uint16_t htons(unint16_t host16bitvalue)` | host byte order -> network byte order (short: 16 bytes) |
| `uint32_t htonl(uint32_t host32bitvalue)` | host byte order -> network byte order (long: 32 bytes); NOT 64 bytes even if the system's long is of 64 bytes. |
| `uint16_t ntohs(uint16_t net16bitvalue)` | network byte order -> host byte order (short: 16 bytes) |
| `uint32_t ntohl(uint32_t ne32bitvalue)` | network byte order -> host byte order (long: 32 bytes) |

# Byte Manipulation Functions

**Lib:** `<string.h>`

There are two groups of functions that operate on multibyte fields, without interpreting the data, and without assuming that the data is a null terminated C string:
1. Functions whos name begin with `b` (for byte), are from `4.2BSD` and are still provided by almost any system that supports the socket functions.
2. Functions whose names begin with `mem` (for memory), are from ANSI C standard and are provided with any system that supports ANSI C library.

| Function | Desc |
| -- | -- |
| `void bzero(void *dest, size_t nbytes)` | Sets the specified number of bytes to 0 in the destination. |
| `void bcopy(const void *src, void *dest, size_t nbytes)` | Moves the specified number of bytes from the src to dest. |
| `int bcmp(const void *ptr1, const void *ptr2, size_t nbytes)` | Compares two arbitrary byte strings. Return value is 0 if both are identical; otherwise, it is nonzero. |
| `void *memset(void *dest, int c, size_t len)` | Sets the specified number of bytes to the value `c` in dest. |
| `void *memcpy(void *dest, const void *src, size_t nbytes)` | Similar to `bcopy`, but order of two pointer args swapped. |
| `int memcmp(const void *ptr1, const void *ptr2, size_t nbytes)` | `bcopy` handles overlapping fields correctly, while the behaviour of `memcpy` is undefines if the src & dest overlap. |

# Internet Address Functions

**Lib:** `<arpa/inet.h>`

| Function | Desc |
| -- | -- |
| `int inet_aton(const char *strptr, struct in_addr *adrptr)` | Converts C char string to 32-bit binary network byte ordered IPv4 value, and puts into `in_addr *` struct; Returns 1 if string was valid, 0 on error; If `*adrptr` is `NULL`, still validates ASCII string. |
| `in_addr_t inet_addr(const char *strptr)` | Same conversion a `inet_aton`; Returns `INADDR_NONE` (`255.255.255.255`) on error, so broadcast IP handling not supported; Deprecated, and should use `inet_aton` or `inet_pton` instead. |
| `char *inet_ntoa(struct in_addr inaddr)` | Converts 32-bit binary network byte ordered IPv4 address to dotted-dcimal string; String pointed by the return value resides in static memory; Takes struct not the struct pointer as the argument. |
| `int inet_pton(int family, const char *strptr, void *addrptr)` | Converts IPv4/IPv6 ASCII string to network-byte ordered binary value and insert in the struct `in_addr`/`in6_addr`; Returns 1 if OK, 0 if input not valid, -1 on error. |
| `const char *inet_ntop(int family, const void *addrptr, car *strptr, size_t len)` | Converts strct `in_addr`/`in6_addr` to ASCII representation; Returns pointer to result if OK, `NULL` on error. |

## `inet_pton` and `inet_ntop`
- The `family` argument for both is either `AF_INET` or `AF_INET6`. If family is not supported, both functions return an error with `errno` set to `EAFNOSUPPORT`.
- `len` argument to `inet_ntop` is the size of the destination, to prevent the function from overflowing the caller's buffer.
  - To help specify the size, the following two definitions are defined by including the `<netinet/in.h>` header:
    ```c
    #define INET_ADDRSTRLEN  16 /* for IPv4 dotted-decimal */
    #define INET6_ADDRSTRLEN 46 /* for IPv6 hex string */
    ```
  - If the `len` is too small to hold the resulting presentation format, including the terminating null, and `NULL` pointer is returned and `errno` is set to `ENOSPC`.
- The `strptr` argument to `inet_ntop` cannot be a null pointer. The caller must allocate memory for the destination and specify its size. On success, this pointer is the return value of the function.