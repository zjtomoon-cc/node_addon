const demoaddon = require('bindings')('demoaddon');
// JS call API
module.exports = { 
  hello : demoaddon.hello
};
