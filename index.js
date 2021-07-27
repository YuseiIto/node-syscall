const syscall = require('./build/Release/node_syscall')
const O_RDONLY=0;

console.log(syscall.getpid());
let fd=syscall.open("testdata.txt",0);
console.log(fd);

console.log(syscall.read(fd,10).toString('utf8'))

console.log(syscall.pipe())
console.log(syscall.gettimeofday())
