package color

import "fmt"

const (
	reset   = "\033[0m"
	red     = "\033[31m"
	green   = "\033[32m"
	yellow  = "\033[33m"
	blue    = "\033[34m"
	magenta = "\033[35m"
	cyan    = "\033[36m"
	gray    = "\033[37m"
	white   = "\033[97m"
)

func RedString(s string, args ...interface{}) string {
	return makeColorString(red, s, args...)
}

func GreenString(s string, args ...interface{}) string {
	return makeColorString(green, s, args...)
}

func YellowString(s string, args ...interface{}) string {
	return makeColorString(yellow, s, args...)
}

func BlueString(s string, args ...interface{}) string {
	return makeColorString(blue, s, args...)
}

func MagentaString(s string, args ...interface{}) string {
	return makeColorString(magenta, s, args...)
}

func CyanString(s string, args ...interface{}) string {
	return makeColorString(cyan, s, args...)
}

func GrayString(s string, args ...interface{}) string {
	return makeColorString(gray, s, args...)
}

func WhiteString(s string, args ...interface{}) string {
	return makeColorString(white, s, args...)
}

func makeColorString(color, s string, args ...interface{}) string {
	msg := fmt.Sprintf(s, args...)
	return fmt.Sprintf("%s%s%s", color, msg, reset)
}
