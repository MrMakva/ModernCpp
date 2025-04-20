package checks

import (
	"os/exec"
	"strings"

	"cpp/ci/internal/config"
)

func CheckCodeStyle(projectDir string, task *config.Task) CheckOutput {
	sourceFiles, _ := CollectProjectFiles(projectDir, "*.cpp")
	headerFiles, _ := CollectProjectFiles(projectDir, "*.hpp")
	allFiles := append(sourceFiles, headerFiles...)

	unformattedFiles := make([]string, 0)
	for _, sourceFile := range allFiles {
		cmd := exec.Command("clang-format", "--Werror", "--dry-run", sourceFile)
		err := cmd.Run()
		if err != nil {
			unformattedFiles = append(unformattedFiles, sourceFile)
		}
	}

	return CheckOutput{
		Success:   len(unformattedFiles) == 0,
		CheckName: "Code Style",
		Details:   listUnformattedFiles(unformattedFiles),
	}
}

func listUnformattedFiles(files []string) []string {
	if len(files) == 0 {
		return make([]string, 0)
	}
	return []string{"Unformatted files: " + strings.Join(files, ", ")}
}
