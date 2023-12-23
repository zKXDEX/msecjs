# mSec

mSec is a Node.js module providing high-precision timing functions. This module is particularly useful in environments where microsecond-level time measurement is required.

## Installation

You can install mSec:

```bash
npm install msec

or 

pnpm install msec

or 

yarn add msec
```

Then, run the installation script defined in package.json:

```bash

npm run install

or

pnpm run install

or

yarn install

```

After installation, run the rebuild script:

```bash
npm run rebuild
or
pnpm run rebuild
or
yarn rebuild
```

## Usage

Here are some examples of how to use mSec in your project:

```javascript
const mSec = require('msec');

// Get the current time in microseconds
let now = mSec.now();
console.log(now);

// Get the current time as a double
let nowDouble = mSec.nowDouble();
console.log(nowDouble);

// Get the current time as an array [seconds, microseconds]
let nowStruct = mSec.nowStruct();
console.log(nowStruct);
```

## API

mSec exposes the following functions:

- `now()`: Returns the current time in microseconds.
- `nowDouble()`: Returns the current time as a double value.
- `nowStruct()`: Returns the current time as an array containing seconds and microseconds.

## Building

This module uses `node-gyp` for building. Ensure you have the necessary tools for compiling Node.js native modules.

## Contributions

Contributions are welcome. Please send your pull requests to this repository.

## License

[MIT](LICENSE)