package checks

import (
	"os"
	"strings"

	"cpp/ci/internal/config"
)

func CheckArtifacts(projectDir string, task *config.Task) CheckOutput {
	missingArtifacts := make([]string, 0)
	for _, artifact := range task.Artifacts {
		_, err := os.Stat(artifact)
		if err != nil {
			missingArtifacts = append(missingArtifacts, artifact)
		}
	}
	return CheckOutput{
		CheckName: "Artifacts",
		Success:   len(missingArtifacts) == 0,
		Details:   []string{"Missing artifacts: " + strings.Join(missingArtifacts, ", ")},
	}
}
