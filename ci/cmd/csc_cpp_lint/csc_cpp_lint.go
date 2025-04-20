package main

import (
	_ "embed"
	"errors"
	"log"
	"os"
	"regexp"

	"cpp/ci/internal/checks"
	"cpp/ci/internal/color"
	"cpp/ci/internal/config"
	"gopkg.in/yaml.v3"
)

//go:embed config.yaml
var configText []byte

func main() {
	var cfg config.App
	err := yaml.Unmarshal(configText, &cfg)
	if err != nil {
		log.Fatal(err)
	}

	wd, _ := os.Getwd()
	task, err := selectTask(cfg, wd)
	if err != nil {
		log.Fatal(err)
	}

	var exitStatus int

	checkList := []func(string, *config.Task) checks.CheckOutput{
		checks.CheckProjectStructure,
		checks.CheckCodeStyle,
		checks.CheckBuild,
		checks.CheckArtifacts,
		checks.RunClangTidy,
	}

	for _, check := range checkList {
		checkOutput := check(wd, task)
		if !checkOutput.Success {
			exitStatus = 1
		}
		printCheckOutput(checkOutput)
	}

	os.Exit(exitStatus)
}

func selectTask(cfg config.App, wd string) (*config.Task, error) {
	for _, t := range cfg.Tasks {
		re, err := regexp.Compile(t.Pattern)
		if err != nil {
			return nil, err
		}
		if re.MatchString(wd) {
			return &t, nil
		}
	}
	return nil, errors.New("Task settings not found")
}

func printCheckOutput(output checks.CheckOutput) {
	if output.Success {
		log.Printf(color.GreenString("%s OK", output.CheckName))
		return
	}
	log.Printf(color.RedString("%s FAIL", output.CheckName))
	for _, detail := range output.Details {
		log.Println(detail)
	}
}
