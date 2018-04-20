package main

import (
	"bytes"
	"io/ioutil"
	"strconv"
	"strings"
)

type TokenType int

const (
	TokenVariable   TokenType = iota
	TokenConstant   TokenType = iota
	TokenAssignment TokenType = iota
	TokenAnd        TokenType = iota
	TokenOr         TokenType = iota
	TokenNot        TokenType = iota
	TokenLeftShift  TokenType = iota
	TokenRightShift TokenType = iota
)

type Token struct {
	Type        TokenType
	StringValue string
	IntValue    uint
}

type ASTNode interface {
	Evaluate() uint
}

type ASTLeftShiftNode struct {
	Operand     ASTNode
	ShiftAmount uint
}

func (n *ASTLeftShiftNode) Evaluate() uint {
	return (n.Operand.Evaluate() << n.ShiftAmount) & 0xFFFF
}

type ASTRightShiftNode struct {
	Operand     ASTNode
	ShiftAmount uint
}

func (n *ASTRightShiftNode) Evaluate() uint {
	return (n.Operand.Evaluate() >> n.ShiftAmount)
}

type ASTAndNode struct {
	Operand1 ASTNode
	Operand2 ASTNode
}

func (n *ASTAndNode) Evaluate() uint {
	return n.Operand1.Evaluate() & n.Operand2.Evaluate()
}

type ASTOrNode struct {
	Operand1 ASTNode
	Operand2 ASTNode
}

func (n *ASTOrNode) Evaluate() uint {
	return n.Operand1.Evaluate() | n.Operand2.Evaluate()
}

type ASTNotNode struct {
	Operand ASTNode
}

func (n *ASTNotNode) Evaluate() uint {
	return (^n.Operand.Evaluate()) & 0xFFFF
}

type ASTConstantNode struct {
	Value uint
}

func (n *ASTConstantNode) Evaluate() uint {
	return n.Value
}

type ASTAssignmentNode struct {
	Name       string
	Evaluated  bool
	Value      uint
	Expression ASTNode
}

func (n *ASTAssignmentNode) Evaluate() uint {
	if !n.Evaluated {
		n.Value = n.Expression.Evaluate()
		n.Evaluated = true
	}
	return n.Value
}

type SymbolTable struct {
	table map[string]*ASTAssignmentNode
}

func (s *SymbolTable) Get(name string) *ASTAssignmentNode {
	if s.table[name] == nil {
		s.table[name] = &ASTAssignmentNode{Name: name}
	}
	return s.table[name]
}

func NewSymbolTable() *SymbolTable {
	return &SymbolTable{make(map[string]*ASTAssignmentNode)}
}

func main() {
	f, err := ioutil.ReadFile("./7.txt")
	if err != nil {
		panic(err)
	}

	tokens := make([]Token, 0, 10000)
	/* Lex the circuit source code, breaking it up into tokens */
	for _, line := range bytes.Split(f, []byte{'\r', '\n'}) {
		for _, token := range strings.Split(string(line), " ") {
			switch token {
			case "->":
				tokens = append(tokens, Token{TokenAssignment, token, 0})
			case "AND":
				tokens = append(tokens, Token{TokenAnd, token, 0})
			case "OR":
				tokens = append(tokens, Token{TokenOr, token, 0})
			case "LSHIFT":
				tokens = append(tokens, Token{TokenLeftShift, token, 0})
			case "RSHIFT":
				tokens = append(tokens, Token{TokenRightShift, token, 0})
			case "NOT":
				tokens = append(tokens, Token{TokenNot, token, 0})
			default:
				i, err := strconv.Atoi(token)
				if err != nil {
					tokens = append(tokens, Token{TokenVariable, token, 0})
				} else {
					tokens = append(tokens, Token{TokenConstant, token, uint(i)})
				}
			}
		}
	}

	/* Parse the lexed tokens into an AST */
	symbols := NewSymbolTable()
	t := 0
	for t < len(tokens) {
		if tokens[t+1].Type == TokenAssignment {
			symbolRes := symbols.Get(tokens[t+2].StringValue)
			if tokens[t].Type == TokenConstant {
				symbolRes.Expression = &ASTConstantNode{tokens[t].IntValue}
			} else {
				symbolRes.Expression = symbols.Get(tokens[t].StringValue)
			}
			t = t + 3
		} else if tokens[t+2].Type == TokenAssignment {
			// assignment unary expression
			symbol := symbols.Get(tokens[t+1].StringValue)
			symbolRes := symbols.Get(tokens[t+3].StringValue)
			symbolRes.Expression = &ASTNotNode{symbol}
			t = t + 4

		} else if tokens[t+3].Type == TokenAssignment {
			// assignment of binary expression
			var symbolL ASTNode
			if tokens[t].Type == TokenConstant {
				symbolL = &ASTConstantNode{tokens[t].IntValue}
			} else {
				symbolL = symbols.Get(tokens[t].StringValue)
			}
			symbolRes := symbols.Get(tokens[t+4].StringValue)

			switch tokens[t+1].Type {
			case TokenAnd:
				symbolRes.Expression = &ASTAndNode{symbolL, symbols.Get(tokens[t+2].StringValue)}
			case TokenOr:
				symbolRes.Expression = &ASTOrNode{symbolL, symbols.Get(tokens[t+2].StringValue)}
			case TokenLeftShift:
				symbolRes.Expression = &ASTLeftShiftNode{symbolL, uint(tokens[t+2].IntValue)}
			case TokenRightShift:
				symbolRes.Expression = &ASTRightShiftNode{symbolL, uint(tokens[t+2].IntValue)}
			}
			t = t + 5
		}
	}

	/* Evaluate the tree at the node in question to get our result */
	println("a: ", symbols.Get("a").Evaluate())
}
