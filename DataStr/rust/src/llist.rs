// Linked list

// Borrowed a lot from
// https://medium.com/@elvis26112009/implementing-linked-lists-in-rust-the-safe-way-8abfb9607c11
//

// It seems implementing those pointer heavy stuffs in rust
// is not a good idea at all...
//

use std::cell::RefCell;
use std::rc::Rc;

// Types
type NodeRef = Rc<RefCell<Node>>;
type NodeOption = Option<NodeRef>;

#[derive(PartialEq, Debug)]
pub struct Node {
	data: String,
	next: NodeOption
}

impl Node {
	pub fn new(text: String) -> NodeRef {
		Rc::new(RefCell::new(Node {
			data: text,
			next: None
		}))
	}
}

impl Drop for Node {
	fn drop(&mut self) {
		println!("Node with this data -> '{}' just dropped", self.data)
	}
}