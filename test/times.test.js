const syscall = require('../build/Release/node_syscall')

test('gettimeofday(2)', () => {

 const timeofday = syscall.gettimeofday();
 console.log(timeofday)
 expect(timeofday).not.toBe(-1);

});

test('time(2)', () => {

 const time = syscall.time();
 console.log(time)
 expect(time).not.toBe(-1);

});
