package main

import (
	"fmt"
	"io/ioutil"
)

var floor int
var basementTransitions []int

func main() {
	f, err := ioutil.ReadFile("./1.txt")
	if err != nil {
		panic(err)
	}
	for i, b := range f {
		switch b {
		case '(':
			floor++
		case ')':
			if floor == 0 {
				basementTransitions = append(basementTransitions, i+1)
			}
			floor--
		}
	}
	fmt.Printf("Santa in on floor %d.\n", floor)
	fmt.Printf("Santa first entered the basement at instruction %d", basementTransitions[0])
}
