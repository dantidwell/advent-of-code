package main

import (
	"bytes"
	"io/ioutil"
	"strconv"
	"strings"
)

func parseCoordinate(s string) (int, int) {
	tokens := strings.Split(s, ",")
	x, _ := strconv.Atoi(tokens[0])
	y, _ := strconv.Atoi(tokens[1])
	return x, y
}

func max(a, b int) int {
	if a >= b {
		return a
	}
	return b
}

func main() {
	f, err := ioutil.ReadFile("./6.txt")
	if err != nil {
		panic(err)
	}

	gridOne := [1000][1000]bool{}
	gridTwo := [1000][1000]int{}
	for _, line := range bytes.Split(f, []byte{'\r', '\n'}) {
		tokens := strings.Split(string(line), " ")
		var t, r, b, l int
		var actionOne func(bool) bool
		var actionTwo func(int) int
		if tokens[0] == "toggle" {
			l, t = parseCoordinate(tokens[1])
			r, b = parseCoordinate(tokens[3])
			actionOne = func(b bool) bool { return !b }
			actionTwo = func(i int) int { return i + 2 }
		} else if tokens[0] == "turn" {
			l, t = parseCoordinate(tokens[2])
			r, b = parseCoordinate(tokens[4])
			if tokens[1] == "on" {
				actionOne = func(b bool) bool { return true }
				actionTwo = func(i int) int { return i + 1 }
			} else if tokens[1] == "off" {
				actionOne = func(b bool) bool { return false }
				actionTwo = func(i int) int { return max(i-1, 0) }
			}
		}
		for j := t; j <= b; j++ {
			for i := l; i <= r; i++ {
				gridOne[j][i] = actionOne(gridOne[j][i])
				gridTwo[j][i] = actionTwo(gridTwo[j][i])
			}
		}
	}
	lightsOn := 0
	totalBrightness := 0
	for j := 0; j < 1000; j++ {
		for i := 0; i < 1000; i++ {
			if gridOne[j][i] {
				lightsOn++
			}
			totalBrightness += gridTwo[j][i]
		}
	}
	println("There are", lightsOn, "lights on.")
	println("The sum of all brightness values is", totalBrightness)
}
