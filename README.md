## Configure
```bash
./configure
```

## When to use `strlen(input)` & when to use `sizeof(input)` ?

`sizeof(buf)` — **"how much room do I have?"**
Compile-time capacity of the array, including the byte for `'\0'`. **Use it whenever a function is going to write into your buffer:**

```c
n = read(sockfd, recvline, sizeof(recvline) - 1);   // -1 leaves room to NUL-terminate
fgets(input, sizeof(input), stdin);
snprintf(buf, sizeof(buf), "%s", s);
```

`strlen(s)` — **"how many bytes do I actually have?"**
Runtime scan to the first `'\0'`, excluding the terminator. **Use it when a function is going to read out of your buffer**:

```c
write(sockfd, input, strlen(input));   // send only the typed bytes, not all 4097
```

**Three gotchas worth remembering:**

1. **`sizeof` dies on a pointer.** Inside `void f(char *buf)` or `void f(char buf[])`, `sizeof(buf)` is 8, not the array size. Only use it where the array declaration is in scope; otherwise pass the size as a parameter.
2. **`strlen` requires NUL-terminated text.** For binary data (or data that came off a socket), there may be no `'\0'` — track the length yourself from `read()`'s return value.
3. **What `read()` gives you isn't a string.** `read()` does not NUL-terminate, so `recvline[n] = 0;` before you `printf`/`strlen` it. That's exactly why you pass `sizeof(recvline) - 1`.

## Error Checking

| Kind of Function | Example | Failure Value | Test |
| -- | -- | -- | -- |
| System call returning int | `read`, `write`, `socket`, `connect` | `-1` | `< 0` |
| stdio returning a pointer | `fgets`, `fopen` | `NULL` | `== NULL` |
| stdio returning int | `fputs`, `putchar`, `printf` | `EOF` | `== EOF` |
| Returns `size_t` | `fread`, `fwrite`, `strlen` | unsigned - never negative | compare to expected count | 