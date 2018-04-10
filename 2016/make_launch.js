const fs = require('fs')

let configurations = [];

for(let i = 1; i <=25; i++) {
  configurations.push({
    name: `${i}`,
    type: "cppvsdbg",
    request: "launch",
    program: `${i}.exe`,
    args: [`${i}.txt`],
    stopAtEntry: false,
    cwd: `\${workspaceFolder}/bin/${i}`,
    environment: [],
    externalConsole: true
  });
}

const lf = fs.openSync('./.vscode/launch.json', 'w');

fs.writeFileSync(lf, JSON.stringify({ 
  version: "0.2.0", 
  configurations
}));

fs.closeSync(lf);