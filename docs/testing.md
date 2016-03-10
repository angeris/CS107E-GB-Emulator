# Testing

## Testing the CPU

### Dumping the opcodes

Under the `roms/` directory, there should be a `cpu_instrs.c` file. This file contains a `gb_short` array with `64kb` of data. This file is a battery of tests for the instruction set that we have just written, and is a compiled version of Shay Green's `blargg` test battery (a bit more on it can be found [here](http://gbdev.gg8.se/wiki/articles/Test_ROMs)). In particular, it does not require GPU support and, as per the documentation
> Information is printed on screen in a way that needs only minimum LCD
> support, and won't hang if LCD output isn't supported at all.
> Specifically, while polling LY to wait for vblank, it will time out if
> it takes too long, so LY always reading back as the same value won't
> hang the test. It's also OK if scrolling isn't supported; in this case,
> text will appear starting at the top of the screen.

Anyways, in order to do this nicely, we require another emulator that at least passes these tests (some of which can be found in the link above); in particular, it should be an open source emulator. In order to test it, all of the register values should be dumped into some text file with some standard format. That is, find the place where the opcodes are executed and, every time an opcode is executed print out both the opcode and the current register values in one line; for example

```python
AF:0xAAAA,BC:0xBBBB,DE:0xCCCC,GH:0xDDDD,SP:0xEEEE,PC:0xFFFF,OP:0xEF
```

This should be step one with the ROM that is provided and some known, working emulator. See if there's an open source Python emulator (probably will be the most legible and easiest to run out-of-the-box) and use that to test it.

### Testing our opcodes
Now, currently, the main loop should only have a step that executes the next opcode (starting at `PC` position `0x150` as per the PanDocs), and then continues to just run where the next step is. It should not handle interrupts (yet! We'll get to that super soon), but that should be more than enough. In paritcular, you should print everything to the serial port (*using the same format as you specified before!*), and [dump all of it into a file, as per this StackOverflow thread](http://stackoverflow.com/questions/4807474/copying-gnu-screen-scrollback-buffer-to-file-extended-hardcopy). Afterwards you can use the `diff` instruction in order to see how our registers differ. Any time there's something wrong, you can essentially assume (though not always, but it's highly unlikely) that the previous instruction is the culprit and go through what the possible problem is.

After all of that is done, we should be good to start incorporating the controller and moving on to incorporating the screen!

Also, please don't hesitate to message me throughout <3

### Checklist
- [ ] Find a working emulator which passes all of the tests. Do this by just running the rom, which will print out a message to the emulator screen, if it doesn't fail.
- [ ] Add a print-to-file statement any time an opcode is executed (probably in the main loop) to dump all of the registers and the current `PC` into a line (there may be some trouble if there are interrupts, but in this case you should just message me, `diff` should just report that we added a crapload of lines in between cases, so I think we should be fine).
- [ ] Print each executed instruction of our emulator into `screen`, then dump the entire `screen` session into a file (I would recommend doing all of this inside of the make-install).
- [ ] Diff out between files and then see what opcodes have caused failures.
- [ ] Go and correct those failures and re-dump the `screen`.