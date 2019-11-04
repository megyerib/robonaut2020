# CubeMX configuration files

```
cubemx/*/*
!cubemx/*/*.ioc
```

Because of this .gitignore pattern **only the .ioc files are under version control!**

Folder structure:

* cubemx
  * project folder
    * project_name.ioc
	* generated files will be put here but won't be commited
  * ... (other projects)