const syscall = require('../build/Release/node_syscall')

test('open(2)', () => {

 const fd = syscall.open("hello.txt",0);
 expect(fd).not.toBe(-1);

});


test('write(2)/close(2)/read(2)', () => {

 const fd = syscall.open("hello.txt",2|8);
 expect(fd).not.toBe(-1);

 const  d=Buffer.alloc(11);
 d.write("Hello World");
 expect(syscall.write(fd,d,11)).toBe(11);
 
 expect(syscall.close(fd)).toBe(0);
 
 const fd2= syscall.open("hello.txt",0);
 expect(fd2).not.toBe(-1);

 const readres=syscall.read(fd2,11).toString();
 console.log(readres);
 expect(readres).toBe("Hello World");
 expect(syscall.close(fd2)).toBe(0);
});
