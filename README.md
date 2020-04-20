# Rom based filter graph

Popular examples of filter graph implementations are media frameworks like gstreamer and DirectShow.
This is a filter graph implementation for extremely small microcontrollers (e.g. 2+k of RAM, 16k ROM). 

The ROM based filter graph is implemented by a number of ROM based lists which are NULL terminated. Each list is maintained in its separate section. These sections need to be alphabetically sorted and consecutively allocated. 
Unfortunately there is no portable way to achieve this. Usually variable attributes handle section assignment and a linker script takes care of correct placement.
