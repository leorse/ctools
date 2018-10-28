# ctools #

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/d8195f8e812e448a9c26da25f0471bcf)](https://www.codacy.com/app/damien-messner/ctools?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=leorse/ctools&amp;utm_campaign=badger)

Some tools to make your C Programs more reliable and profilable.

## Development ##
Still in development, adding some new feature...

## Documentation ##

- Plain C
- Use __MALLOC insted of malloc. The new allocated pointer is now a part of the internal variable of ctools
- Use __SPRINTF instead of sprintf and tell you if the destination array is too small for the input
- Use __STRCPY instead of strcpy and tell you if the destination array is too small for the source array
- (not yet) Use __MEMSET instead of memset and tell you if the destination array is too small for size

Use of valgrind on test exec show the same error as ctools: valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./ctools 

To debug it: 'gdb ctools', 'layout next' while on source view
