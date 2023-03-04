package main
import "C"

// notice：//export xxxx is necessary

//export Hello
func Hello(_name *C.char) *C.char {
	// args string type，return string type
	name := C.GoString(_name)
	
	res := "hello"
	if len(name) > 0 {
	    res += "," + name
	}
	
	return C.CString(res)
}

func main() {
	
}
