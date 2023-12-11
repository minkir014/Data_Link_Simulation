package datalinksimulation;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=431,299");
    submodules:
        coordinator: Coordinator {
            @display("p=220,195");
        }
        Node0: Node {
            @display("p=377,117");
        }
        Node1: Node {
            @display("p=64,123");
        }
    connections:
        coordinator.out0 --> Node0.in_coordinator;
        coordinator.out1 --> Node1.in_coordinator;
        Node0.out --> Node1.in_node;
        Node1.out --> Node0.in_node;
}

