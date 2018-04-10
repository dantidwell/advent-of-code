package main

import (
	"crypto/md5"
	"fmt"
	"strconv"
)

var key = []byte("yzbqklnj")
var prefix = []byte{0, 0, 0}

func main() {
	i := int64(1)
	for {
		h := md5.New()
		h.Write(strconv.AppendInt(key, i, 10))
		hash := h.Sum(nil)
		if hash[0] == 0 && hash[1] == 0 && hash[2] == 0 {
			break
		}
		i++
	}
	fmt.Printf("Hash found after %d iterations.", i)
}
