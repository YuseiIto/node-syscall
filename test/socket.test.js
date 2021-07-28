const syscall = require('../build/Release/node_syscall')

test('socket(2)', () => {

 const sockfd = syscall.socket(syscall.AF_UNIX,syscall.SOCK_STREAM,0);
 expect(sockfd).not.toBe(-1);

});
