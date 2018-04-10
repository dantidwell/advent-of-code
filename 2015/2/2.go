package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
)

const NewLine = "\r\n"
const EndOfFile = 0x00
const Times = 0x78

var Empty = make([]byte, 0)

func min(x, y, z int) int {
	var temp int
	if x <= y {
		temp = x
	} else {
		temp = y
	}
	if temp <= z {
		return temp
	}
	return z
}

func getDimensions(stream []byte) (int, int, int) {
	dims := make([]int, 3)
	for i, r := range bytes.Split(stream, []byte{Times}) {
		for _, b := range r {
			dims[i] = dims[i]*10 + (int(b) - 0x30)
		}
	}
	return dims[0], dims[1], dims[2]
}

func getNextBox(stream []byte) ([]byte, []byte) {
	nlIdx := bytes.IndexAny(stream, NewLine)
	if nlIdx == -1 {
		return stream[0:len(stream)], Empty
	} else {
		return stream[0:nlIdx], stream[nlIdx+2:]
	}
}

func main() {
	f, err := ioutil.ReadFile("./2.txt")
	if err != nil {
		panic(err)
	}

	totalPaper := 0
	totalRibbon := 0
	for len(f) > 0 {
		var d []byte

		d, f = getNextBox(f)
		l, w, h := getDimensions(d)

		totalPaper += 2*l*w + 2*l*h + 2*w*h + min(l*w, l*h, w*h)
		totalRibbon += min(2*(l+w), 2*(l+h), 2*(w+h)) + (l * w * h)
	}
	fmt.Printf("The elves need %d square feet of wrapping paper.\n", totalPaper)
	fmt.Printf("The elves need %d linear feet of ribbon.", totalRibbon)
}
