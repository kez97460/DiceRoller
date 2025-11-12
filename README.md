# Simple dice roller

This repo contains a simple CLI to roll dice, made in C (following the C17 standard). It only requires basic functions from glibc to work.

## Installing

Simply clone the repo and execute the following command to install the binary into /usr/local/bin :

```bash
sudo make install
```

or you can simply compile it by calling `make`, the binary can then be found in the `bin` directory.

## Using the software

Once installed, a quick guide on using the software can be found by using the help flag :

```bash
roll -h
```

### Examples

```bash
roll 1d8+5

roll 1d20+1d4+5 -a
```

### Flags

| Flag     | Signification |
| -------- | ------------- |
| -a | roll with advantage (reroll next d20, take best result) |
| -d | roll with disadvantage (reroll next d20, take worse result) |
| -h | show help |

## License

The main software is under the MIT license (see LICENSE.md). 

The `easyargs.h` file was not developed by me, and can be found [here](https://github.com/gouwsxander/easy-args). See [the relevant directory](./easyargs/) for more information.