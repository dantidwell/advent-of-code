package main

import (
	"fmt"
	"io/ioutil"
)

type Point struct {
	x int
	y int
}

func main() {
	f, err := ioutil.ReadFile("./3.txt")
	if err != nil {
		panic(err)
	}

	santas := []Point{{x: 0, y: 0}, {x: 0, y: 0}}

	m := make(map[Point]bool)
	m[santas[0]] = true
	m[santas[1]] = true

	for i, d := range f {
		switch d {
		case '<':
			santas[i%2].x--
		case '>':
			santas[i%2].x++
		case '^':
			santas[i%2].y++
		case 'v':
			santas[i%2].y--
		}
		m[santas[i%2]] = true
	}
	fmt.Printf("Santa's final position is (%d, %d).\n", santas[0].x, santas[0].y)
	fmt.Printf("RoboSanta's final position is (%d, %d).\n", santas[1].x, santas[1].y)
	fmt.Printf("They visited %d houses.", len(m))
}
