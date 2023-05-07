# trace_translation
The srouce code provided is for translating *.pout or *.vout generated from the [memory tracing tool](https://github.com/dgist-datalab/valgrind_cachetrace.git) to the format that can be accepted by the [MQSim-CXL simulator](https://github.com/spypaul/MQSim_CXL.git).
The source code is a simple C++ code that can be compiled by any environment. 

For example, you can compile it using g++ like the following:
```
$ g++ Source.cpp -o Source.out
```
To start the translation process, place the *.pout or *.vout files into the same directory as the complied executable and run:

```
$ ./Source.out
```

The output file will be generated in the same directory, and this generated trace file can be used in the simulator. 
For more details about the simulator, please check the [MQSim-CXL simulator](https://github.com/spypaul/MQSim_CXL.git).
