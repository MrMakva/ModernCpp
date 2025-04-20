package checks

type CheckOutput struct {
	Success   bool
	CheckName string
	Details   []string
}
