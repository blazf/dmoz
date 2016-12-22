/**
 * Copyright (c) 2015, Jozef Stefan Institute, Quintelligence d.o.o. and contributors
 * All rights reserved.
 *
 * This source code is licensed under the FreeBSD license found in the
 * LICENSE file in the root directory of this source tree.
 */

var pathDmozBinary = __dirname + '/out/dmoz.node';
var foundBinary = require('fs').existsSync(pathDmozBinary);

if (foundBinary) {
    module.exports = exports = require(pathDmozBinary);
} else {
    throw new Error('qminer binary not found!');
}
