package main

import (
	"bytes"
	"io/ioutil"
)

func main() {
	f, err := ioutil.ReadFile("./5.txt")
	if err != nil {
		panic(err)
	}
	numNiceStringsPt1 := 0
	numNiceStringsPt2 := 0
	for _, l := range bytes.Split(f, []byte{'\n'}) {
		var li_1, li_2 byte

		hasRepeat := false
		hasBadString := false
		hasPalindrome := false
		pairs := make(map[string]int)
		vowels := 0
		for i := 0; i < len(l); i++ {
			li := l[i]

			/* check for vowels */
			if li == 'a' || li == 'e' || li == 'i' || li == 'o' || li == 'u' {
				vowels++
			}

			/* check for non-overlapping pairs */
			if !(li == li_1 && li_1 == li_2) {
				pairs[string([]byte{li_1, li})]++
			}

			/* check for repeats, bad strings and palindromes */
			hasBadString = hasBadString || (li_1 == 'a' && li == 'b' || li_1 == 'c' && li == 'd' || li_1 == 'p' && li == 'q' || li_1 == 'x' && li == 'y')
			hasPalindrome = hasPalindrome || (li == li_2)
			hasRepeat = hasRepeat || (li == li_1)

			li_2 = li_1
			li_1 = li
		}

		hasDoublePair := false
		for _, value := range pairs {
			hasDoublePair = hasDoublePair || value == 2
		}
		if vowels >= 3 && hasRepeat && !hasBadString {
			numNiceStringsPt1++
		}
		if hasDoublePair && hasPalindrome {
			numNiceStringsPt2++
		}
	}

	println("There are", numNiceStringsPt1, "nice strings in Part 1.")
	println("There are", numNiceStringsPt2, "nice strings in Part 2.")
}
