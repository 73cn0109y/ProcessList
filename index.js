var addon = require('bindings')('processList');

console.log(addon.getProcesses());