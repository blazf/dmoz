#ifndef DMOZ_JS_H
#define DMOZ_JS_H

#ifndef BUILDING_NODE_EXTENSION
    #define BUILDING_NODE_EXTENSION
#endif

#include <node.h>
#include <node_object_wrap.h>

#include <base.h>
#include <mine.h>
#include <nodeutil.h>

/**
* DMoz module
* @module dmoz
* @example
* // import module
* var dmoz = require('dmoz');
*/
class TJsDmoz : public node::ObjectWrap {
  public:
    // Node framework
    static void Init(v8::Local<v8::Object> exports);

  private:
    static PDMozCfy DMozCfy;

  private:
    /// Parse dmoz bow files and create classifier according to the filter
    JsDeclareFunction(initDmoz);
    /// Load classifier
    JsDeclareFunction(loadDmoz);
    /// Classify
    JsDeclareFunction(classify);

};

#endif
