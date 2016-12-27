/**
 * Copyright (c) 2015, Jozef Stefan Institute, Quintelligence d.o.o. and contributors
 * All rights reserved.
 *
 * This source code is licensed under the FreeBSD license found in the
 * LICENSE file in the root directory of this source tree.
 */

const fs = require('fs');

// constructor
module.exports = class Dmoz {

    constructor(params) {
        let self = this;

        // load binary module
        const pathDmozBinary = __dirname + '/out/dmoz.node';
        if (fs.existsSync(pathDmozBinary)) {
            self._classifier = require(pathDmozBinary);
        } else {
            throw new Error('dmoz binary not found!');
        }

        // check if classifier file exists
        if (fs.existsSync(params.classifier)) {
            // it does, just load it
            self._classifier.load(params);
        } else {
            // it does not, first create it from complete dmoz
            self._classifier.init(params);
        }

        // load filter
        let filters = fs.readFileSync(params.filter, "utf8").split("\n");
        self.partials = [ ];
        for (let filter of filters) {
            // we only care about paltiar filters
            if (filter.indexOf("*") != -1) {
                // split on prefix and suffix
                let partial = filter.split("*");
                self.partials.push(partial);
            }
        }
        console.log(self.partials.length);
    }

    _cleanCategory(category) {
        let self = this;
        // go over and see if we match a partial
        for (let partial of self.partials) {
            // if we match both prefix and suffix, then we found the match
            if (category.startsWith(partial[0]) && category.endsWith(partial[1])) {
                return partial[1];
            }
        }
        // if no match, just remove the "Top/"
        return category.slice(4);
    }

    classify(text) {
        let self = this;
        // classify document
        let results = self._classifier.classify(text, 1);
        // we just care about top category
        let topCat = results.categories.length > 0 ? results.categories[0] : "Top/";
        // clean the name and return it
        return self._cleanCategory(topCat);
    }
}
