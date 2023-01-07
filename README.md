# AsyncCallQueue
AsyncCallQueue is a container class that allows to call functions though a thread and its function call loop. This is similar 
to std::async functionality but guarantee that calls are issued in order and with the same thread. Implementation is header 
 only.

    - support global functions
    - member functions
    - lambdas
    
    