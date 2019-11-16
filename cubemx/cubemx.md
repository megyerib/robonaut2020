# CubeMX configuration files

## Location of drivers downloaded by CubeMX

`C:\Users\user\STM32Cube\Repository`

##.gitignore

```
cubemx/*/*
!cubemx/*/*.ioc
```

Because of this pattern **only the .ioc files are under version control!**

Folder structure:

- cubemx
  - project folder
    - project_name.ioc
	- generated files will be put here but will be ignored
  - ... (other projects)
