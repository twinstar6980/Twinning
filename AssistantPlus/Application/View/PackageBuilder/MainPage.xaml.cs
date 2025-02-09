#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using System.Text.Json.Nodes;
using Microsoft.UI.Xaml.Navigation;
using Windows.ApplicationModel.DataTransfer;
using FluentIconGlyph = AssistantPlus.Control.FluentIconGlyph;

namespace AssistantPlus.View.PackageBuilder {

	public sealed partial class MainPage : Page, Home.IModulePage {

		#region life

		public MainPage (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			this.Controller.Initialize();
		}

		// ----------------

		protected override void OnNavigatedTo (
			NavigationEventArgs args
		) {
			_ = this.ModulePageApplyOption(args.Parameter.As<List<String>>());
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		private MainPageController Controller { get; }

		#endregion

		#region module page

		public Task ModulePageApplyOption (
			List<String> optionView
		) {
			return this.Controller.ApplyOption(optionView);
		}

		public Task<List<String>> ModulePageCollectOption (
		) {
			return this.Controller.CollectOption();
		}

		public Task<Boolean> ModulePageRequestClose (
		) {
			return this.Controller.RequestClose();
		}

		#endregion

	}

	public class MainPageController : CustomController, Home.IModulePageController {

		#region data

		public MainPage View { get; init; } = default!;

		// ----------------

		public String? ProjectDirectory { get; set; } = null;

		public ProjectSetting? ProjectSetting { get; set; } = null;

		// ----------------

		[MemberNotNullWhen(true, nameof(MainPageController.ProjectDirectory))]
		[MemberNotNullWhen(true, nameof(MainPageController.ProjectSetting))]
		public Boolean IsLoaded {
			get {
				return this.ProjectDirectory != null;
			}
		}

		#endregion

		#region initialize

		public void Initialize (
		) {
			return;
		}

		public async Task ApplyOption (
			List<String> optionView
		) {
			await ControlHelper.WaitUntilLoaded(this.View);
			var optionProjectDirectory = default(String?);
			try {
				var option = new CommandLineReader(optionView);
				if (option.Check("-ProjectDirectory")) {
					optionProjectDirectory = option.NextString();
				}
				if (!option.Done()) {
					throw new ($"Too many option : '{String.Join(' ', option.NextStringList())}'.");
				}
			}
			catch (Exception e) {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			if (optionProjectDirectory != null) {
				await this.ApplyLoad(optionProjectDirectory);
			}
			return;
		}

		public async Task<List<String>> CollectOption (
		) {
			var option = new CommandLineWriter();
			if (option.Check("-ProjectDirectory", this.ProjectDirectory != null)) {
				option.NextString(this.ProjectDirectory.AsNotNull());
			}
			return option.Done();
		}

		public async Task<Boolean> RequestClose (
		) {
			return true;
		}

		// ----------------

		public async Task ApplyLoad (
			String projectDirectory
		) {
			if (this.IsLoaded) {
				await this.ProjectClose();
			}
			await this.ProjectOpen(projectDirectory);
			if (this.IsLoaded) {
				await App.Instance.AppendRecentLauncherItem(new () {
					Title = Regex.Replace(StorageHelper.Name(projectDirectory), @"(\.pvz2_package_project)$", "", RegexOptions.IgnoreCase),
					Type = ModuleType.PackageBuilder,
					Option = await this.CollectOption(),
					Command = [],
				});
			}
			return;
		}

		#endregion

		#region common

		public static String DataViewFormatForGroup = $"{nameof(AssistantPlus)}.{nameof(PackageBuilder)}.Group";

		public static String DataViewFormatForResource = $"{nameof(AssistantPlus)}.{nameof(PackageBuilder)}.Resource";

		// ----------------

		public Boolean WorkerState { get; set; } = false;

		public async Task WorkerExecuteCommand (
			List<String> argument
		) {
			this.WorkerState = true;
			this.NotifyPropertyChanged(
				nameof(this.uWorkerProgress_IsActive),
				nameof(this.uWorkerButton_Style)
			);
			this.View.uWorkerButton.Flyout.ShowAt(this.View.uWorkerButton);
			var result = await this.View.uWorkerPage.ExecuteCommand(argument);
			if (result != null && result.First() == "s") {
				App.MainWindow.PushNotification(InfoBarSeverity.Success, "Execute succeeded", "");
			}
			else {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Execute failed", "");
			}
			this.WorkerState = false;
			this.NotifyPropertyChanged(
				nameof(this.uWorkerProgress_IsActive),
				nameof(this.uWorkerButton_Style)
			);
			return;
		}

		// ----------------

		public async Task WorkerDoTranspile (
			List<String>? targetScope,
			Boolean?      mode
		) {
			GF.AssertTest(this.IsLoaded);
			if (this.View.uPackageList.SelectedItems.Count != 1) {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Please select single package target", "");
				return;
			}
			await this.WorkerExecuteCommand(ModdingWorker.ForwardHelper.MakeArgumentForCommand(
				null,
				"pvz2.package_project.transpile",
				mode == null
					? new ([
						new ("project_directory", this.MakeScopeRootPath()),
						new ("target_package", String.Join('|', this.View.uPackageList.SelectedItems.First().As<MainPagePackageItemController>().Setting.Name)),
						new ("target_scope", targetScope == null ? "*" : String.Join('|', targetScope)),
					])
					: new ([
						new ("project_directory", this.MakeScopeRootPath()),
						new ("target_package", String.Join('|', this.View.uPackageList.SelectedItems.First().As<MainPagePackageItemController>().Setting.Name)),
						new ("target_scope", targetScope == null ? "*" : String.Join('|', targetScope)),
						new ("option_generalize_rton", !mode),
						new ("option_generalize_ptx", !mode),
						new ("option_generalize_pam", !mode),
						new ("option_generalize_wem", !mode),
						new ("option_specialize_rton", mode),
						new ("option_specialize_ptx", mode),
						new ("option_specialize_pam", mode),
						new ("option_specialize_wem", mode),
					])
			));
			return;
		}

		public async Task WorkerDoCompile (
			List<String>? targetScope
		) {
			GF.AssertTest(this.IsLoaded);
			await this.WorkerExecuteCommand(ModdingWorker.ForwardHelper.MakeArgumentForCommand(
				null,
				"pvz2.package_project.compile",
				new ([
					new ("project_directory", this.MakeScopeRootPath()),
					new ("target_package", String.Join('|', this.View.uPackageList.SelectedItems.Select(GF.As<MainPagePackageItemController>).Select((value) => (value.Setting.Name)))),
					new ("target_scope", targetScope == null ? "*" : String.Join('|', targetScope)),
				])
			));
			return;
		}

		public async Task WorkerDoLink (
			Boolean remakeManifest
		) {
			GF.AssertTest(this.IsLoaded);
			await this.WorkerExecuteCommand(ModdingWorker.ForwardHelper.MakeArgumentForCommand(
				null,
				"pvz2.package_project.link",
				new ([
					new ("project_directory", this.MakeScopeRootPath()),
					new ("target_package", String.Join('|', this.View.uPackageList.SelectedItems.Select(GF.As<MainPagePackageItemController>).Select((value) => (value.Setting.Name)))),
					new ("remake_manifest", remakeManifest),
				])
			));
			return;
		}

		// ----------------

		public async Task<Boolean> CheckVersionFile (
			String projectDirectory
		) {
			try {
				var versionFile = $"{projectDirectory}/version.txt";
				var versionText = await StorageHelper.ReadFileText(versionFile);
				GF.AssertTest(versionText == "2");
				return true;
			}
			catch (Exception) {
				return false;
			}
		}

		// ----------------

		public String MakeScopeRootPath (
		) {
			GF.AssertTest(this.IsLoaded);
			var projectDirectory = this.ProjectDirectory;
			return $"{projectDirectory}";
		}

		public String MakeScopeRootPath (
			String part
		) {
			GF.AssertTest(this.IsLoaded);
			var projectDirectory = this.ProjectDirectory;
			return $"{projectDirectory}/{part}";
		}

		public String MakeScopeRootPath (
			String part,
			String group
		) {
			GF.AssertTest(this.IsLoaded);
			var projectDirectory = this.ProjectDirectory;
			return $"{projectDirectory}/{part}/{group}";
		}

		public String MakeScopeRootPath (
			String part,
			String group,
			String resource
		) {
			GF.AssertTest(this.IsLoaded);
			var projectDirectory = this.ProjectDirectory;
			return $"{projectDirectory}/{part}/{group}/{resource}";
		}

		public String MakeScopeChildPath (
			String parent,
			String name
		) {
			GF.AssertTest(this.IsLoaded);
			return $"{parent}/{name}";
		}

		public String MakeScopeSettingPath (
			String parent
		) {
			GF.AssertTest(this.IsLoaded);
			return $"{parent}/setting.json";
		}

		public List<String> ListScopeChildName (
			String parent
		) {
			GF.AssertTest(this.IsLoaded);
			return StorageHelper.ListDirectory(parent, 0).Where((value) => (!value.StartsWith("."))).ToList();
		}

		public String FindAvailableScopeChildName (
			String parent,
			String name
		) {
			GF.AssertTest(this.IsLoaded);
			var index = 0;
			var nameRequest = name;
			while (StorageHelper.ExistDirectory($"{parent}/{nameRequest}")) {
				index++;
				nameRequest = $"{name}~{index}";
			}
			return nameRequest;
		}

		// ----------------

		public MainPagePartItemController FindScopeNode (
			String part
		) {
			GF.AssertTest(this.IsLoaded);
			return this.uPartList_ItemsSource.First((value) => (value.Name == part));
		}

		public MainPageGroupItemController FindScopeNode (
			String part,
			String group
		) {
			GF.AssertTest(this.IsLoaded);
			return this.FindScopeNode(part).Children.First((value) => (value.Name == group));
		}

		public MainPageResourceItemController FindScopeNode (
			String part,
			String group,
			String resource
		) {
			GF.AssertTest(this.IsLoaded);
			return this.FindScopeNode(part, group).Children.First((value) => (value.Name == resource));
		}

		// ----------------

		public async Task ProjectOpen (
			String projectDirectory
		) {
			GF.AssertTest(!this.IsLoaded);
			if (!(await this.CheckVersionFile(projectDirectory))) {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Failed to check version.txt", "");
				return;
			}
			var hideDialog = await ControlHelper.ShowDialogForWait(this.View);
			this.ProjectDirectory = projectDirectory;
			this.NotifyPropertyChanged(
				nameof(this.uLoadedAction_IsEnabled),
				nameof(this.uProjectDirectory_Text)
			);
			await this.ProjectReload();
			await hideDialog();
			return;
		}

		public async Task ProjectClose (
		) {
			GF.AssertTest(this.IsLoaded);
			this.uPackageList_ItemsSource.Clear();
			this.uPartList_ItemsSource.Clear();
			this.uGroupList_ItemsSource.Clear();
			this.uResourceList_ItemsSource.Clear();
			this.ProjectDirectory = null;
			this.NotifyPropertyChanged(
				nameof(this.uLoadedAction_IsEnabled),
				nameof(this.uProjectDirectory_Text)
			);
			// NOTE : refresh CollectionViewSource to prevent exception when virtual node loading
			this.NotifyPropertyChanged(
				nameof(this.uGroupList_ItemsSource),
				nameof(this.uResourceList_ItemsSource)
			);
			return;
		}

		public async Task ProjectReload (
		) {
			GF.AssertTest(this.IsLoaded);
			var projectDirectory = this.MakeScopeRootPath();
			GF.AssertTest(StorageHelper.ExistDirectory(projectDirectory));
			this.uPackageList_ItemsSource.Clear();
			this.uPartList_ItemsSource.Clear();
			this.uGroupList_ItemsSource.Clear();
			this.uResourceList_ItemsSource.Clear();
			this.ProjectSetting = await JsonHelper.DeserializeFile<ProjectSetting>(this.MakeScopeSettingPath(projectDirectory));
			foreach (var packageSetting in this.ProjectSetting.Package) {
				var packageNode = new MainPagePackageItemController() {
					Host = this,
					Setting = packageSetting,
				};
				this.uPackageList_ItemsSource.Add(packageNode);
			}
			foreach (var partName in this.ListScopeChildName(projectDirectory)) {
				var partDirectory = this.MakeScopeChildPath(projectDirectory, partName);
				var partSetting = await JsonHelper.DeserializeFile<PartSetting>(this.MakeScopeSettingPath(partDirectory));
				var partNode = new MainPagePartItemController() {
					Host = this,
					Name = partName,
					Setting = partSetting,
					Children = [],
				};
				foreach (var groupName in this.ListScopeChildName(partDirectory)) {
					var groupDirectory = this.MakeScopeChildPath(partDirectory, groupName);
					var groupSetting = await JsonHelper.DeserializeFile<GroupSetting>(this.MakeScopeSettingPath(groupDirectory));
					var groupNode = new MainPageGroupItemController() {
						Host = this,
						Name = groupName,
						Setting = groupSetting,
						Parent = partNode,
						Children = [],
					};
					foreach (var resourceName in this.ListScopeChildName(groupDirectory)) {
						var resourceDirectory = this.MakeScopeChildPath(groupDirectory, resourceName);
						var resourceSetting = await JsonHelper.DeserializeFile<ResourceSetting>(this.MakeScopeSettingPath(resourceDirectory));
						var resourceNode = new MainPageResourceItemController() {
							Host = this,
							Name = resourceName,
							Setting = resourceSetting,
							Parent = groupNode,
						};
						groupNode.Children.Add(resourceNode);
					}
					partNode.Children.Add(groupNode);
				}
				this.uPartList_ItemsSource.Add(partNode);
			}
			return;
		}

		public async Task ProjectReveal (
			Boolean forSetting
		) {
			GF.AssertTest(this.IsLoaded);
			var sourceDirectory = this.MakeScopeRootPath();
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			if (!forSetting) {
				await StorageHelper.RevealDirectory(sourceDirectory);
			}
			else {
				await StorageHelper.RevealFile(this.MakeScopeSettingPath(sourceDirectory));
			}
			return;
		}

		public async Task ProjectSaveSetting (
		) {
			GF.AssertTest(this.IsLoaded);
			await JsonHelper.SerializeFile(this.MakeScopeSettingPath(this.MakeScopeRootPath()), this.ProjectSetting);
			return;
		}

		// ----------------

		public async Task PackageAppend (
		) {
			GF.AssertTest(this.IsLoaded);
			var destinationPackage = new PackageSetting() {
				Name = "__",
				Part = [],
				Version = new () {
					Number = 4,
					ExtendedTextureInformationForPvz2Cn = 0,
				},
				Compression = new () {
					General = true,
					Texture = true,
					Filter = [],
				},
				Manifest = new () {
					Type = PackageManifestType.Internal,
					Suffix = "",
				},
				Category = new () {
					Resolution = [],
					Locale = [],
				},
				Conversion = new () {
					Rton = [],
					Ptx = [],
					Pam = [],
					Wem = [],
				},
				Variable = [],
			};
			this.ProjectSetting.Package.Add(destinationPackage);
			await this.ProjectSaveSetting();
			var itemNode = new MainPagePackageItemController() {
				Host = this,
				Setting = destinationPackage,
			};
			this.uPackageList_ItemsSource.Add(itemNode);
			this.View.uPackageList.SelectedItems.Add(itemNode);
			return;
		}

		public async Task PackageRename (
			PackageSetting sourcePackage,
			String         destinationPackage
		) {
			GF.AssertTest(this.IsLoaded);
			sourcePackage.Name = destinationPackage;
			var itemNode = this.uPackageList_ItemsSource.First((value) => (Object.ReferenceEquals(value.Setting, sourcePackage)));
			itemNode.NotifyPropertyChanged(
				nameof(itemNode.uName_Text)
			);
			await this.ProjectSaveSetting();
			return;
		}

		public async Task PackageDelete (
			PackageSetting sourcePackage
		) {
			GF.AssertTest(this.IsLoaded);
			this.ProjectSetting.Package.Remove(sourcePackage);
			await this.ProjectSaveSetting();
			var itemNode = this.uPackageList_ItemsSource.First((value) => (Object.ReferenceEquals(value.Setting, sourcePackage)));
			this.uPackageList_ItemsSource.Remove(itemNode);
			return;
		}

		public async Task PackageCopy (
			PackageSetting sourcePackage
		) {
			GF.AssertTest(this.IsLoaded);
			var destinationPackage = new PackageSetting() {
				Name = sourcePackage.Name + "~copy",
				Part = sourcePackage.Part.ToList(),
				Version = new () {
					Number = sourcePackage.Version.Number,
					ExtendedTextureInformationForPvz2Cn = sourcePackage.Version.ExtendedTextureInformationForPvz2Cn,
				},
				Compression = new () {
					General = sourcePackage.Compression.General,
					Texture = sourcePackage.Compression.Texture,
					Filter = sourcePackage.Compression.Filter.ToList(),
				},
				Manifest = new () {
					Type = sourcePackage.Manifest.Type,
					Suffix = sourcePackage.Manifest.Suffix,
				},
				Category = new () {
					Resolution = sourcePackage.Category.Resolution.ToList(),
					Locale = sourcePackage.Category.Locale.ToList(),
				},
				Conversion = JsonHelper.DeepCopy(sourcePackage.Conversion),
				Variable = sourcePackage.Variable.Select((value) => (new Variable() { Name = value.Name, Value = value.Value })).ToList(),
			};
			this.ProjectSetting.Package.Add(destinationPackage);
			await this.ProjectSaveSetting();
			var itemNode = new MainPagePackageItemController() {
				Host = this,
				Setting = destinationPackage,
			};
			this.uPackageList_ItemsSource.Add(itemNode);
			return;
		}

		// ----------------

		public async Task PartReload (
			String  sourcePart,
			Boolean forRefresh = true
		) {
			GF.AssertTest(this.IsLoaded);
			var partDirectory = this.MakeScopeRootPath(sourcePart);
			GF.AssertTest(StorageHelper.ExistDirectory(partDirectory));
			var partSetting = await JsonHelper.DeserializeFile<PartSetting>(this.MakeScopeSettingPath(partDirectory));
			var partNode = default(MainPagePartItemController);
			if (!forRefresh) {
				partNode = new () {
					Host = this,
					Name = sourcePart,
					Setting = partSetting,
					Children = [],
				};
			}
			else {
				partNode = this.FindScopeNode(sourcePart);
				partNode.Setting = partSetting;
				partNode.Children.Clear();
			}
			foreach (var groupName in this.ListScopeChildName(partDirectory)) {
				var groupDirectory = this.MakeScopeChildPath(partDirectory, groupName);
				var groupSetting = await JsonHelper.DeserializeFile<GroupSetting>(this.MakeScopeSettingPath(groupDirectory));
				var groupNode = new MainPageGroupItemController() {
					Host = this,
					Name = groupName,
					Setting = groupSetting,
					Parent = partNode,
					Children = [],
				};
				foreach (var resourceName in this.ListScopeChildName(groupDirectory)) {
					var resourceDirectory = this.MakeScopeChildPath(groupDirectory, resourceName);
					var resourceSetting = await JsonHelper.DeserializeFile<ResourceSetting>(this.MakeScopeSettingPath(resourceDirectory));
					var resourceNode = new MainPageResourceItemController() {
						Host = this,
						Name = resourceName,
						Setting = resourceSetting,
						Parent = groupNode,
					};
					groupNode.Children.Add(resourceNode);
				}
				partNode.Children.Add(groupNode);
			}
			if (!forRefresh) {
				this.uPartList_ItemsSource.Add(partNode);
			}
			else {
				partNode.NotifyPropertyChanged(
					nameof(partNode.uCount_Value)
				);
			}
			return;
		}

		public async Task PartReveal (
			String  sourcePart,
			Boolean forSetting
		) {
			GF.AssertTest(this.IsLoaded);
			var sourceDirectory = this.MakeScopeRootPath(sourcePart);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			if (!forSetting) {
				await StorageHelper.RevealDirectory(sourceDirectory);
			}
			else {
				await StorageHelper.RevealFile(this.MakeScopeSettingPath(sourceDirectory));
			}
			return;
		}

		public async Task PartAppend (
		) {
			GF.AssertTest(this.IsLoaded);
			var destinationPart = this.FindAvailableScopeChildName(this.MakeScopeRootPath(), "__");
			var destinationDirectory = this.MakeScopeRootPath(destinationPart);
			GF.AssertTest(!StorageHelper.ExistDirectory(destinationDirectory));
			var destinationSetting = new PartSetting() {
				Variable = [],
			};
			StorageHelper.CreateDirectory(destinationDirectory);
			await JsonHelper.SerializeFile(this.MakeScopeSettingPath(destinationDirectory), destinationSetting);
			var itemNode = new MainPagePartItemController() {
				Host = this,
				Name = destinationPart,
				Setting = destinationSetting,
				Children = [],
			};
			this.uPartList_ItemsSource.Add(itemNode);
			this.View.uPartList.SelectedItems.Add(itemNode);
			return;
		}

		public async Task PartRename (
			String sourcePart,
			String destinationPart
		) {
			GF.AssertTest(this.IsLoaded);
			if (destinationPart == sourcePart) {
				return;
			}
			var sourceDirectory = this.MakeScopeRootPath(sourcePart);
			var destinationDirectory = this.MakeScopeRootPath(destinationPart);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			GF.AssertTest(!StorageHelper.ExistDirectory(destinationDirectory));
			StorageHelper.RenameDirectory(sourceDirectory, destinationDirectory);
			var itemNode = this.FindScopeNode(sourcePart);
			itemNode.Name = destinationPart;
			itemNode.NotifyPropertyChanged(
				nameof(itemNode.uName_Text)
			);
			foreach (var groupNode in this.uGroupList_ItemsSource.Where((value) => (value.PartName == sourcePart))) {
				groupNode.PartName = destinationPart;
				groupNode.NotifyPropertyChanged(
					nameof(groupNode.uName_Text)
				);
			}
			foreach (var groupNode in this.uResourceList_ItemsSource.Where((value) => (value.PartName == sourcePart))) {
				groupNode.PartName = destinationPart;
				groupNode.NotifyPropertyChanged(
					nameof(groupNode.uName_Text)
				);
			}
			return;
		}

		public async Task PartDelete (
			String sourcePart
		) {
			GF.AssertTest(this.IsLoaded);
			var sourceDirectory = this.MakeScopeRootPath(sourcePart);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			StorageHelper.TrashDirectory(sourceDirectory);
			var itemNode = this.FindScopeNode(sourcePart);
			this.uPartList_ItemsSource.Remove(itemNode);
			foreach (var packageNode in this.uPackageList_ItemsSource) {
				packageNode.Setting.Part.RemoveAll((value) => (value == sourcePart));
				packageNode.NotifyPropertyChanged(
					nameof(packageNode.uCount_Value)
				);
			}
			await this.ProjectSaveSetting();
			return;
		}

		public async Task PartCopy (
			String sourcePart
		) {
			GF.AssertTest(this.IsLoaded);
			var destinationPart = this.FindAvailableScopeChildName(this.MakeScopeRootPath(), sourcePart);
			var sourceDirectory = this.MakeScopeRootPath(sourcePart);
			var destinationDirectory = this.MakeScopeRootPath(destinationPart);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			GF.AssertTest(!StorageHelper.ExistDirectory(destinationDirectory));
			StorageHelper.CopyDirectory(sourceDirectory, destinationDirectory);
			await this.PartReload(destinationPart, false);
			return;
		}

		// ----------------

		public async Task GroupReload (
			String  sourcePart,
			String  sourceGroup,
			Boolean forRefresh = true
		) {
			GF.AssertTest(this.IsLoaded);
			var groupDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup);
			GF.AssertTest(StorageHelper.ExistDirectory(groupDirectory));
			var groupSetting = await JsonHelper.DeserializeFile<GroupSetting>(this.MakeScopeSettingPath(groupDirectory));
			var groupNode = default(MainPageGroupItemController)!;
			if (!forRefresh) {
				groupNode = new () {
					Host = this,
					Name = sourceGroup,
					Setting = groupSetting,
					Parent = this.FindScopeNode(sourcePart),
					Children = [],
				};
			}
			else {
				groupNode = this.FindScopeNode(sourcePart, sourceGroup);
				groupNode.Setting = groupSetting;
				groupNode.Children.Clear();
			}
			foreach (var resourceName in this.ListScopeChildName(groupDirectory)) {
				var resourceDirectory = this.MakeScopeChildPath(groupDirectory, resourceName);
				var resourceSetting = await JsonHelper.DeserializeFile<ResourceSetting>(this.MakeScopeSettingPath(resourceDirectory));
				var resourceNode = new MainPageResourceItemController() {
					Host = this,
					Name = resourceName,
					Setting = resourceSetting,
					Parent = groupNode,
				};
				groupNode.Children.Add(resourceNode);
			}
			if (!forRefresh) {
				groupNode.Parent.Children.Add(groupNode);
			}
			else {
				groupNode.NotifyPropertyChanged(
					nameof(groupNode.uCount_Value)
				);
			}
			return;
		}

		public async Task GroupReveal (
			String  sourcePart,
			String  sourceGroup,
			Boolean forSetting
		) {
			GF.AssertTest(this.IsLoaded);
			var sourceDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			if (!forSetting) {
				await StorageHelper.RevealDirectory(sourceDirectory);
			}
			else {
				await StorageHelper.RevealFile(this.MakeScopeSettingPath(sourceDirectory));
			}
			return;
		}

		public async Task GroupAppend (
			String sourcePart
		) {
			GF.AssertTest(this.IsLoaded);
			var destinationGroup = this.FindAvailableScopeChildName(this.MakeScopeRootPath(sourcePart), "__");
			var destinationDirectory = this.MakeScopeRootPath(sourcePart, destinationGroup);
			GF.AssertTest(!StorageHelper.ExistDirectory(destinationDirectory));
			var destinationSetting = new GroupSetting() {
				Variable = [],
			};
			StorageHelper.CreateDirectory(destinationDirectory);
			await JsonHelper.SerializeFile(this.MakeScopeSettingPath(destinationDirectory), destinationSetting);
			var parentNode = this.FindScopeNode(sourcePart);
			var itemNode = new MainPageGroupItemController() {
				Host = this,
				Name = destinationGroup,
				Setting = destinationSetting,
				Parent = parentNode,
				Children = [],
			};
			parentNode.Children.Add(itemNode);
			parentNode.NotifyPropertyChanged(
				nameof(parentNode.uCount_Value)
			);
			this.View.uGroupList.SelectedItems.Add(itemNode);
			return;
		}

		public async Task GroupRename (
			String sourcePart,
			String sourceGroup,
			String destinationGroup
		) {
			GF.AssertTest(this.IsLoaded);
			if (destinationGroup == sourceGroup) {
				return;
			}
			var sourceDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup);
			var destinationDirectory = this.MakeScopeRootPath(sourcePart, destinationGroup);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			GF.AssertTest(!StorageHelper.ExistDirectory(destinationDirectory));
			StorageHelper.RenameDirectory(sourceDirectory, destinationDirectory);
			var itemNode = this.FindScopeNode(sourcePart, sourceGroup);
			itemNode.Name = destinationGroup;
			itemNode.NotifyPropertyChanged(
				nameof(itemNode.uName_Text)
			);
			foreach (var groupNode in this.uResourceList_ItemsSource.Where((value) => (value.PartName == sourcePart && value.GroupName == sourceGroup))) {
				groupNode.GroupName = destinationGroup;
				groupNode.NotifyPropertyChanged(
					nameof(groupNode.uName_Text)
				);
			}
			return;
		}

		public async Task GroupDelete (
			String sourcePart,
			String sourceGroup
		) {
			GF.AssertTest(this.IsLoaded);
			var sourceDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			StorageHelper.TrashDirectory(sourceDirectory);
			var itemNode = this.FindScopeNode(sourcePart, sourceGroup);
			itemNode.Parent.Children.Remove(itemNode);
			itemNode.Parent.NotifyPropertyChanged(
				nameof(itemNode.Parent.uCount_Value)
			);
			return;
		}

		public async Task GroupCopy (
			String sourcePart,
			String sourceGroup
		) {
			GF.AssertTest(this.IsLoaded);
			var destinationGroup = this.FindAvailableScopeChildName(this.MakeScopeRootPath(sourcePart), sourceGroup);
			var sourceDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup);
			var destinationDirectory = this.MakeScopeRootPath(sourcePart, destinationGroup);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			GF.AssertTest(!StorageHelper.ExistDirectory(destinationDirectory));
			StorageHelper.CopyDirectory(sourceDirectory, destinationDirectory);
			await this.GroupReload(sourcePart, destinationGroup, false);
			return;
		}

		public async Task GroupMove (
			String sourcePart,
			String sourceGroup,
			String destinationPart
		) {
			GF.AssertTest(this.IsLoaded);
			if (destinationPart == sourcePart) {
				return;
			}
			var sourceDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup);
			var destinationDirectory = this.MakeScopeRootPath(destinationPart, sourceGroup);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			GF.AssertTest(!StorageHelper.ExistDirectory(destinationDirectory));
			StorageHelper.RenameDirectory(sourceDirectory, destinationDirectory);
			var itemNode = this.FindScopeNode(sourcePart, sourceGroup);
			itemNode.Parent.Children.Remove(itemNode);
			itemNode.Parent.NotifyPropertyChanged(
				nameof(itemNode.Parent.uCount_Value)
			);
			itemNode.Parent = this.FindScopeNode(destinationPart);
			itemNode.Parent.Children.Add(itemNode);
			itemNode.Parent.NotifyPropertyChanged(
				nameof(itemNode.Parent.uCount_Value)
			);
			return;
		}

		// ----------------

		public async Task ResourceReload (
			String  sourcePart,
			String  sourceGroup,
			String  sourceResource,
			Boolean forRefresh = true
		) {
			GF.AssertTest(this.IsLoaded);
			var resourceDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup, sourceResource);
			GF.AssertTest(StorageHelper.ExistDirectory(resourceDirectory));
			var resourceSetting = await JsonHelper.DeserializeFile<ResourceSetting>(this.MakeScopeSettingPath(resourceDirectory));
			var resourceNode = default(MainPageResourceItemController);
			if (!forRefresh) {
				resourceNode = new () {
					Host = this,
					Name = sourceResource,
					Setting = resourceSetting,
					Parent = this.FindScopeNode(sourcePart, sourceGroup),
				};
			}
			else {
				resourceNode = this.FindScopeNode(sourcePart, sourceGroup, sourceResource);
				resourceNode.Setting = resourceSetting;
			}
			if (!forRefresh) {
				resourceNode.Parent.Children.Add(resourceNode);
			}
			else {
				resourceNode.NotifyPropertyChanged(
					nameof(resourceNode.uCategory_Style),
					nameof(resourceNode.uCategoryResolution_Text),
					nameof(resourceNode.uCategoryLocale_Text),
					nameof(resourceNode.uTypeIcon_Glyph),
					nameof(resourceNode.uTypeText_Text)
				);
			}
			return;
		}

		public async Task ResourceReveal (
			String  sourcePart,
			String  sourceGroup,
			String  sourceResource,
			Boolean forSetting
		) {
			GF.AssertTest(this.IsLoaded);
			var sourceDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup, sourceResource);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			if (!forSetting) {
				await StorageHelper.RevealDirectory(sourceDirectory);
			}
			else {
				await StorageHelper.RevealFile(this.MakeScopeSettingPath(sourceDirectory));
			}
			return;
		}

		public async Task ResourceAppend (
			String sourcePart,
			String sourceGroup
		) {
			GF.AssertTest(this.IsLoaded);
			var destinationResource = this.FindAvailableScopeChildName(this.MakeScopeRootPath(sourcePart, sourceGroup), "__");
			var destinationDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup, destinationResource);
			GF.AssertTest(!StorageHelper.ExistDirectory(destinationDirectory));
			var destinationSetting = new ResourceSetting() {
				Category = new () {
					Resolution = null,
					Locale = null,
				},
				Type = ResourceType.Dummy,
				Property = new JsonObject(),
				Variable = [],
			};
			StorageHelper.CreateDirectory(destinationDirectory);
			await JsonHelper.SerializeFile(this.MakeScopeSettingPath(destinationDirectory), destinationSetting);
			var parentNode = this.FindScopeNode(sourcePart, sourceGroup);
			var itemNode = new MainPageResourceItemController() {
				Host = this,
				Name = destinationResource,
				Setting = destinationSetting,
				Parent = parentNode,
			};
			parentNode.Children.Add(itemNode);
			parentNode.NotifyPropertyChanged(
				nameof(parentNode.uCount_Value)
			);
			this.View.uResourceList.SelectedItems.Add(itemNode);
			return;
		}

		public async Task ResourceRename (
			String sourcePart,
			String sourceGroup,
			String sourceResource,
			String destinationResource
		) {
			GF.AssertTest(this.IsLoaded);
			if (destinationResource == sourceResource) {
				return;
			}
			var sourceDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup, sourceResource);
			var destinationDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup, destinationResource);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			GF.AssertTest(!StorageHelper.ExistDirectory(destinationDirectory));
			StorageHelper.RenameDirectory(sourceDirectory, destinationDirectory);
			var itemNode = this.FindScopeNode(sourcePart, sourceGroup, sourceResource);
			itemNode.Name = destinationResource;
			itemNode.NotifyPropertyChanged(
				nameof(itemNode.uName_Text)
			);
			return;
		}

		public async Task ResourceDelete (
			String sourcePart,
			String sourceGroup,
			String sourceResource
		) {
			GF.AssertTest(this.IsLoaded);
			var sourceDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup, sourceResource);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			StorageHelper.TrashDirectory(sourceDirectory);
			var itemNode = this.FindScopeNode(sourcePart, sourceGroup, sourceResource);
			itemNode.Parent.Children.Remove(itemNode);
			itemNode.Parent.NotifyPropertyChanged(
				nameof(itemNode.Parent.uCount_Value)
			);
			return;
		}

		public async Task ResourceCopy (
			String sourcePart,
			String sourceGroup,
			String sourceResource
		) {
			GF.AssertTest(this.IsLoaded);
			var destinationResource = this.FindAvailableScopeChildName(this.MakeScopeRootPath(sourcePart, sourceGroup), sourceResource);
			var sourceDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup, sourceResource);
			var destinationDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup, destinationResource);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			GF.AssertTest(!StorageHelper.ExistDirectory(destinationDirectory));
			StorageHelper.CopyDirectory(sourceDirectory, destinationDirectory);
			await this.ResourceReload(sourcePart, sourceGroup, destinationResource, false);
			return;
		}

		public async Task ResourceMove (
			String sourcePart,
			String sourceGroup,
			String sourceResource,
			String destinationPart,
			String destinationGroup
		) {
			GF.AssertTest(this.IsLoaded);
			if (destinationPart == sourcePart && destinationGroup == sourceGroup) {
				return;
			}
			var sourceDirectory = this.MakeScopeRootPath(sourcePart, sourceGroup, sourceResource);
			var destinationDirectory = this.MakeScopeRootPath(destinationPart, destinationGroup, sourceResource);
			GF.AssertTest(StorageHelper.ExistDirectory(sourceDirectory));
			GF.AssertTest(!StorageHelper.ExistDirectory(destinationDirectory));
			StorageHelper.RenameDirectory(sourceDirectory, destinationDirectory);
			var itemNode = this.FindScopeNode(sourcePart, sourceGroup, sourceResource);
			itemNode.Parent.Children.Remove(itemNode);
			itemNode.Parent.NotifyPropertyChanged(
				nameof(itemNode.Parent.uCount_Value)
			);
			itemNode.Parent = this.FindScopeNode(destinationPart, destinationGroup);
			itemNode.Parent.Children.Add(itemNode);
			itemNode.Parent.NotifyPropertyChanged(
				nameof(itemNode.Parent.uCount_Value)
			);
			return;
		}

		#endregion

		#region page

		public async void uPage_DragOver (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<Page>();
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.AcceptedOperation = DataPackageOperation.Link;
			}
			return;
		}

		public async void uPage_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<Page>();
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.Handled = true;
				var item = await args.DataView.GetStorageItemsAsync();
				if (item.Count != 1) {
					App.MainWindow.PushNotification(InfoBarSeverity.Error, "Source is multiply.", "");
					return;
				}
				var projectDirectory = StorageHelper.GetLongPath(item[0].Path);
				if (!StorageHelper.ExistDirectory(projectDirectory)) {
					App.MainWindow.PushNotification(InfoBarSeverity.Error, "Source is not a directory.", "");
					return;
				}
				await this.ApplyLoad(projectDirectory);
			}
			return;
		}

		#endregion

		#region worker

		public Boolean uWorkerProgress_IsActive {
			get {
				return this.WorkerState;
			}
		}

		public Style uWorkerButton_Style {
			get {
				return this.View.FindResource(!this.WorkerState ? "DefaultButtonStyle" : "AccentButtonStyle").As<Style>();
			}
		}

		#endregion

		#region loaded action

		public Boolean uLoadedAction_IsEnabled {
			get {
				return this.IsLoaded;
			}
		}

		#endregion

		#region project

		public String uProjectDirectory_Text {
			get {
				if (!this.IsLoaded) {
					return "";
				}
				return this.ProjectDirectory;
			}
		}

		public async void uProjectOpen_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			var projectDirectory = await StorageHelper.PickLoadDirectory(App.MainWindow, $"{nameof(PackageBuilder)}.ProjectDirectory");
			if (projectDirectory != null) {
				await this.ApplyLoad(projectDirectory);
			}
			return;
		}

		public async void uProjectClose_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			GF.AssertTest(this.IsLoaded);
			await this.ProjectClose();
			return;
		}

		// ----------------

		public async void uProjectAction_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			switch (senders.Tag.As<String>()) {
				case "Reload": {
					await this.ProjectReload();
					break;
				}
				case "RevealRoot": {
					await this.ProjectReveal(false);
					break;
				}
				case "RevealSetting": {
					await this.ProjectReveal(true);
					break;
				}
				case "TranspileCustom": {
					await this.WorkerDoTranspile(null, null);
					await this.ProjectReload();
					break;
				}
				case "TranspileGeneralize": {
					await this.WorkerDoTranspile(null, false);
					await this.ProjectReload();
					break;
				}
				case "TranspileSpecialize": {
					await this.WorkerDoTranspile(null, true);
					await this.ProjectReload();
					break;
				}
				case "Compile": {
					await this.WorkerDoCompile(null);
					break;
				}
				case "LinkLegacy": {
					await this.WorkerDoLink(false);
					break;
				}
				case "LinkRemake": {
					await this.WorkerDoLink(true);
					break;
				}
				default: throw new ();
			}
			return;
		}

		#endregion

		#region package

		public async void uPackageList_DragItemsCompleted (
			Object                      sender,
			DragItemsCompletedEventArgs args
		) {
			var senders = sender.As<ListView>();
			GF.AssertTest(this.IsLoaded);
			this.ProjectSetting.Package = this.uPackageList_ItemsSource.Select((value) => (value.Setting)).ToList();
			await this.ProjectSaveSetting();
			return;
		}

		public ObservableCollection<MainPagePackageItemController> uPackageList_ItemsSource { get; } = [];

		public async void uPackageList_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ListView>();
			GF.AssertTest(this.IsLoaded);
			return;
		}

		// ----------------

		public async void uPackageAction_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			switch (senders.Tag.As<String>()) {
				case "Delete": {
					foreach (var item in this.View.uPackageList.SelectedItems.Select(GF.As<MainPagePackageItemController>).ToList()) {
						await this.PackageDelete(item.Setting);
					}
					break;
				}
				case "Copy": {
					foreach (var item in this.View.uPackageList.SelectedItems.Select(GF.As<MainPagePackageItemController>).ToList()) {
						await this.PackageCopy(item.Setting);
					}
					break;
				}
				default: throw new ();
			}
			return;
		}

		public async void uPackageActionAppend_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			GF.AssertTest(this.IsLoaded);
			await this.PackageAppend();
			return;
		}

		#endregion

		#region part

		public ObservableCollection<MainPagePartItemController> uPartList_ItemsSource { get; } = [];

		public async void uPartList_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ListView>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in args.RemovedItems.Select(GF.As<MainPagePartItemController>)) {
				this.uGroupList_ItemsSource.Remove(this.uGroupList_ItemsSource.First((value) => (value.PartName == item.Name)));
			}
			foreach (var item in args.AddedItems.Select(GF.As<MainPagePartItemController>)) {
				this.uGroupList_ItemsSource.Add(new () {
					Host = this,
					PartName = item.Name,
					Children = item.Children,
				});
			}
			return;
		}

		// ----------------

		public async void uPartAction_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			switch (senders.Tag.As<String>()) {
				case "Reload": {
					foreach (var item in this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).ToList()) {
						await this.PartReload(item.Name);
					}
					break;
				}
				case "RevealRoot": {
					foreach (var item in this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).ToList()) {
						await this.PartReveal(item.Name, false);
					}
					break;
				}
				case "RevealSetting": {
					foreach (var item in this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).ToList()) {
						await this.PartReveal(item.Name, true);
					}
					break;
				}
				case "Delete": {
					foreach (var item in this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).ToList()) {
						await this.PartDelete(item.Name);
					}
					break;
				}
				case "Copy": {
					foreach (var item in this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).ToList()) {
						await this.PartCopy(item.Name);
					}
					break;
				}
				case "TranspileCustom": {
					await this.WorkerDoTranspile(this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).Select((value) => ($"/{value.Name}")).ToList(), null);
					foreach (var item in this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).ToList()) {
						await this.PartReload(item.Name);
					}
					break;
				}
				case "TranspileGeneralize": {
					await this.WorkerDoTranspile(this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).Select((value) => ($"/{value.Name}")).ToList(), false);
					foreach (var item in this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).ToList()) {
						await this.PartReload(item.Name);
					}
					break;
				}
				case "TranspileSpecialize": {
					await this.WorkerDoTranspile(this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).Select((value) => ($"/{value.Name}")).ToList(), true);
					foreach (var item in this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).ToList()) {
						await this.PartReload(item.Name);
					}
					break;
				}
				case "Compile": {
					await this.WorkerDoCompile(this.View.uPartList.SelectedItems.Select(GF.As<MainPagePartItemController>).Select((value) => ($"/{value.Name}")).ToList());
					break;
				}
				default: throw new ();
			}
			return;
		}

		public async void uPartActionAppend_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			GF.AssertTest(this.IsLoaded);
			await this.PartAppend();
			return;
		}

		#endregion

		#region group

		public async void uGroupList_DragItemsStarting (
			Object                     sender,
			DragItemsStartingEventArgs args
		) {
			var senders = sender.As<ListView>();
			GF.AssertTest(this.IsLoaded);
			args.Data.SetData(MainPageController.DataViewFormatForGroup, String.Join('\n', args.Items.Select(GF.As<MainPageGroupItemController>).Select((value) => ($"{value.Parent.Name}/{value.Name}"))));
			return;
		}

		public ObservableCollection<MainPageGroupGroupController> uGroupList_ItemsSource { get; } = [];

		public async void uGroupList_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ListView>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in args.RemovedItems.Select(GF.As<MainPageGroupItemController>)) {
				this.uResourceList_ItemsSource.Remove(this.uResourceList_ItemsSource.First((value) => (value.PartName == item.Parent.Name && value.GroupName == item.Name)));
			}
			foreach (var item in args.AddedItems.Select(GF.As<MainPageGroupItemController>)) {
				this.uResourceList_ItemsSource.Add(new () {
					Host = this,
					PartName = item.Parent.Name,
					GroupName = item.Name,
					Children = item.Children,
				});
			}
			return;
		}

		// ----------------

		public async void uGroupAction_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			switch (senders.Tag.As<String>()) {
				case "Reload": {
					foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).ToList()) {
						await this.GroupReload(item.Parent.Name, item.Name);
					}
					break;
				}
				case "RevealRoot": {
					foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).ToList()) {
						await this.GroupReveal(item.Parent.Name, item.Name, false);
					}
					break;
				}
				case "RevealSetting": {
					foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).ToList()) {
						await this.GroupReveal(item.Parent.Name, item.Name, true);
					}
					break;
				}
				case "Delete": {
					foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).ToList()) {
						await this.GroupDelete(item.Parent.Name, item.Name);
					}
					break;
				}
				case "Copy": {
					foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).ToList()) {
						await this.GroupCopy(item.Parent.Name, item.Name);
					}
					break;
				}
				case "TranspileCustom": {
					await this.WorkerDoTranspile(this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).Select((value) => ($"/{value.Parent.Name}/{value.Name}")).ToList(), null);
					foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).ToList()) {
						await this.GroupReload(item.Parent.Name, item.Name);
					}
					break;
				}
				case "TranspileGeneralize": {
					await this.WorkerDoTranspile(this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).Select((value) => ($"/{value.Parent.Name}/{value.Name}")).ToList(), false);
					foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).ToList()) {
						await this.GroupReload(item.Parent.Name, item.Name);
					}
					break;
				}
				case "TranspileSpecialize": {
					await this.WorkerDoTranspile(this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).Select((value) => ($"/{value.Parent.Name}/{value.Name}")).ToList(), true);
					foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).ToList()) {
						await this.GroupReload(item.Parent.Name, item.Name);
					}
					break;
				}
				case "Compile": {
					await this.WorkerDoCompile(this.View.uGroupList.SelectedItems.Select(GF.As<MainPageGroupItemController>).Select((value) => ($"/{value.Parent.Name}/{value.Name}")).ToList());
					break;
				}
				default: throw new ();
			}
			return;
		}

		#endregion

		#region resource

		public ObservableCollection<MainPageResourceGroupController> uResourceList_ItemsSource { get; } = [];

		public async void uResourceList_DragItemsStarting (
			Object                     sender,
			DragItemsStartingEventArgs args
		) {
			var senders = sender.As<ListView>();
			GF.AssertTest(this.IsLoaded);
			args.Data.SetData(MainPageController.DataViewFormatForResource, String.Join('\n', args.Items.Select(GF.As<MainPageResourceItemController>).Select((value) => ($"{value.Parent.Parent.Name}/{value.Parent.Name}/{value.Name}"))));
			return;
		}

		// ----------------

		public async void uResourceAction_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			switch (senders.Tag.As<String>()) {
				case "Reload": {
					foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).ToList()) {
						await this.ResourceReload(item.Parent.Parent.Name, item.Parent.Name, item.Name);
					}
					break;
				}
				case "RevealRoot": {
					foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).ToList()) {
						await this.ResourceReveal(item.Parent.Parent.Name, item.Parent.Name, item.Name, false);
					}
					break;
				}
				case "RevealSetting": {
					foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).ToList()) {
						await this.ResourceReveal(item.Parent.Parent.Name, item.Parent.Name, item.Name, true);
					}
					break;
				}
				case "Delete": {
					foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).ToList()) {
						await this.ResourceDelete(item.Parent.Parent.Name, item.Parent.Name, item.Name);
					}
					break;
				}
				case "Copy": {
					foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).ToList()) {
						await this.ResourceCopy(item.Parent.Parent.Name, item.Parent.Name, item.Name);
					}
					break;
				}
				case "TranspileCustom": {
					await this.WorkerDoTranspile(this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).Select((value) => ($"/{value.Parent.Parent.Name}/{value.Parent.Name}/{value.Name}")).ToList(), null);
					foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).ToList()) {
						await this.ResourceReload(item.Parent.Parent.Name, item.Parent.Name, item.Name);
					}
					break;
				}
				case "TranspileGeneralize": {
					await this.WorkerDoTranspile(this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).Select((value) => ($"/{value.Parent.Parent.Name}/{value.Parent.Name}/{value.Name}")).ToList(), false);
					foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).ToList()) {
						await this.ResourceReload(item.Parent.Parent.Name, item.Parent.Name, item.Name);
					}
					break;
				}
				case "TranspileSpecialize": {
					await this.WorkerDoTranspile(this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).Select((value) => ($"/{value.Parent.Parent.Name}/{value.Parent.Name}/{value.Name}")).ToList(), true);
					foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).ToList()) {
						await this.ResourceReload(item.Parent.Parent.Name, item.Parent.Name, item.Name);
					}
					break;
				}
				case "Compile": {
					await this.WorkerDoCompile(this.View.uResourceList.SelectedItems.Select(GF.As<MainPageResourceItemController>).Select((value) => ($"/{value.Parent.Parent.Name}/{value.Parent.Name}/{value.Name}")).ToList());
					break;
				}
				default: throw new ();
			}
			return;
		}

		#endregion

	}

	public class MainPagePackageItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public PackageSetting Setting { get; set; } = default!;

		// ----------------

		public Boolean IsNameEditing { get; set; } = false;

		// ----------------

		public async Task SaveSetting (
		) {
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.ProjectSaveSetting();
			return;
		}

		#endregion

		#region view

		public Boolean uName_Visibility {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return !this.IsNameEditing;
			}
		}

		public String uName_Text {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Setting.Name;
			}
		}

		public Boolean uNameEdit_Visibility {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.IsNameEditing;
			}
		}

		public Boolean uNameEdit_IsEnabled {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.IsNameEditing;
			}
		}

		public async void uNameEdit_IsEnabledChanged (
			Object                             sender,
			DependencyPropertyChangedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			if (senders.IsEnabled) {
				senders.Focus(FocusState.Pointer);
			}
			return;
		}

		public async void uNameEdit_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			this.IsNameEditing = false;
			this.NotifyPropertyChanged(
				nameof(this.uName_Visibility),
				nameof(this.uNameEdit_Visibility),
				nameof(this.uNameEdit_IsEnabled)
			);
			var value = senders.Text;
			if (StorageHelper.CheckName(value)) {
				await this.Host.PackageRename(this.Setting, value);
			}
			return;
		}

		public Size uCount_Value {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Setting.Part.Count;
			}
		}

		// ----------------

		public async void uAction_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			switch (senders.Tag.As<String>()) {
				case "Rename": {
					this.IsNameEditing = true;
					this.NotifyPropertyChanged(
						nameof(this.uName_Text),
						nameof(this.uName_Visibility),
						nameof(this.uNameEdit_Visibility),
						nameof(this.uNameEdit_IsEnabled)
					);
					break;
				}
				case "Delete": {
					await this.Host.PackageDelete(this.Setting);
					break;
				}
				case "Copy": {
					await this.Host.PackageCopy(this.Setting);
					break;
				}
				case "Part": {
					var partList = this.Host.uPartList_ItemsSource.Select((value) => (value.Name)).ToList();
					await ControlHelper.ShowDialogAsFixed(this.Host.View, "Package Part", new PackagePartPanel() {
						Source = partList,
						Value = this.Setting.Part,
						Stamp = UniqueStamp.Create(),
					}, null);
					this.Setting.Part = partList.Where((value) => (this.Setting.Part.Contains(value))).ToList();
					await this.SaveSetting();
					this.NotifyPropertyChanged(
						nameof(this.uCount_Value)
					);
					break;
				}
				case "Setting": {
					await ControlHelper.ShowDialogAsFixed(this.Host.View, "Package Setting", new PackageSettingPanel() {
						Value = this.Setting,
						Stamp = UniqueStamp.Create(),
					}, null);
					await this.SaveSetting();
					break;
				}
				case "Variable": {
					await ControlHelper.ShowDialogAsFixed(this.Host.View, "Package Variable", new VariableListPanel() {
						Value = this.Setting.Variable,
						Stamp = UniqueStamp.Create(),
					}, null);
					await this.SaveSetting();
					break;
				}
				default: throw new ();
			}
			return;
		}

		#endregion

	}

	public class MainPagePartItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public String Name { get; set; } = default!;

		public PartSetting Setting { get; set; } = default!;

		// ----------------

		public ObservableCollection<MainPageGroupItemController> Children { get; set; } = default!;

		// ----------------

		public Boolean IsNameEditing { get; set; } = false;

		// ----------------

		public async Task SaveSetting (
		) {
			GF.AssertTest(this.Host.IsLoaded);
			await JsonHelper.SerializeFile(this.Host.MakeScopeSettingPath(this.Host.MakeScopeRootPath(this.Name)), this.Setting);
			return;
		}

		#endregion

		#region view

		public async void uRoot_DragOver (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<Panel>();
			GF.AssertTest(this.Host.IsLoaded);
			if (args.DataView.Contains(MainPageController.DataViewFormatForGroup)) {
				args.AcceptedOperation = DataPackageOperation.Move;
			}
			return;
		}

		public async void uRoot_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<Panel>();
			GF.AssertTest(this.Host.IsLoaded);
			if (args.DataView.Contains(MainPageController.DataViewFormatForGroup)) {
				args.Handled = true;
				var data = (await args.DataView.GetDataAsync(MainPageController.DataViewFormatForGroup)).As<String>();
				if (data.Length != 0) {
					var itemList = data.Split('\n');
					foreach (var item in itemList) {
						var scope = item.Split('/');
						await this.Host.GroupMove(scope[0], scope[1], this.Name);
					}
				}
			}
			return;
		}

		// ----------------

		public Boolean uName_Visibility {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return !this.IsNameEditing;
			}
		}

		public String uName_Text {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Name;
			}
		}

		public Boolean uNameEdit_Visibility {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.IsNameEditing;
			}
		}

		public Boolean uNameEdit_IsEnabled {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.IsNameEditing;
			}
		}

		public async void uNameEdit_IsEnabledChanged (
			Object                             sender,
			DependencyPropertyChangedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			if (senders.IsEnabled) {
				senders.Focus(FocusState.Pointer);
			}
			return;
		}

		public async void uNameEdit_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			this.IsNameEditing = false;
			this.NotifyPropertyChanged(
				nameof(this.uName_Visibility),
				nameof(this.uNameEdit_Visibility),
				nameof(this.uNameEdit_IsEnabled)
			);
			var value = senders.Text;
			if (StorageHelper.CheckName(value)) {
				await this.Host.PartRename(this.Name, value);
			}
			return;
		}

		public Size uCount_Value {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Children.Count;
			}
		}

		// ----------------

		public async void uAction_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			switch (senders.Tag.As<String>()) {
				case "Reload": {
					await this.Host.PartReload(this.Name);
					break;
				}
				case "RevealRoot": {
					await this.Host.PartReveal(this.Name, false);
					break;
				}
				case "RevealSetting": {
					await this.Host.PartReveal(this.Name, true);
					break;
				}
				case "Rename": {
					this.IsNameEditing = true;
					this.NotifyPropertyChanged(
						nameof(this.uName_Text),
						nameof(this.uName_Visibility),
						nameof(this.uNameEdit_Visibility),
						nameof(this.uNameEdit_IsEnabled)
					);
					break;
				}
				case "Delete": {
					await this.Host.PartDelete(this.Name);
					break;
				}
				case "Copy": {
					await this.Host.PartCopy(this.Name);
					break;
				}
				case "TranspileCustom": {
					await this.Host.WorkerDoTranspile([$"/{this.Name}"], null);
					await this.Host.PartReload(this.Name);
					break;
				}
				case "TranspileGeneralize": {
					await this.Host.WorkerDoTranspile([$"/{this.Name}"], false);
					await this.Host.PartReload(this.Name);
					break;
				}
				case "TranspileSpecialize": {
					await this.Host.WorkerDoTranspile([$"/{this.Name}"], true);
					await this.Host.PartReload(this.Name);
					break;
				}
				case "Compile": {
					await this.Host.WorkerDoCompile([$"/{this.Name}"]);
					break;
				}
				case "Variable": {
					await ControlHelper.ShowDialogAsFixed(this.Host.View, "Part Variable", new VariableListPanel() {
						Value = this.Setting.Variable,
						Stamp = UniqueStamp.Create(),
					}, null);
					await this.SaveSetting();
					break;
				}
				default: throw new ();
			}
			return;
		}

		#endregion

	}

	public class MainPageGroupGroupController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public String PartName { get; set; } = default!;

		// ----------------

		public ObservableCollection<MainPageGroupItemController> Children { get; set; } = default!;

		#endregion

		#region view

		public String uName_Text {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return $"{this.PartName}";
			}
		}

		// ----------------

		public async void uActionAppend_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.GroupAppend(this.PartName);
			return;
		}

		#endregion

	}

	public class MainPageGroupItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public String Name { get; set; } = default!;

		public GroupSetting Setting { get; set; } = default!;

		// ----------------

		public MainPagePartItemController Parent { get; set; } = default!;

		public ObservableCollection<MainPageResourceItemController> Children { get; set; } = default!;

		// ----------------

		public Boolean IsNameEditing { get; set; } = false;

		// ----------------

		public async Task SaveSetting (
		) {
			GF.AssertTest(this.Host.IsLoaded);
			await JsonHelper.SerializeFile(this.Host.MakeScopeSettingPath(this.Host.MakeScopeRootPath(this.Parent.Name, this.Name)), this.Setting);
			return;
		}

		#endregion

		#region view

		public async void uRoot_DragOver (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<Panel>();
			GF.AssertTest(this.Host.IsLoaded);
			if (args.DataView.Contains(MainPageController.DataViewFormatForResource)) {
				args.AcceptedOperation = DataPackageOperation.Move;
			}
			return;
		}

		public async void uRoot_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.As<Panel>();
			GF.AssertTest(this.Host.IsLoaded);
			if (args.DataView.Contains(MainPageController.DataViewFormatForResource)) {
				args.Handled = true;
				var data = (await args.DataView.GetDataAsync(MainPageController.DataViewFormatForResource)).As<String>();
				if (data.Length != 0) {
					var itemList = data.Split('\n');
					foreach (var item in itemList) {
						var scope = item.Split('/');
						await this.Host.ResourceMove(scope[0], scope[1], scope[2], this.Parent.Name, this.Name);
					}
				}
			}
			return;
		}

		// ----------------

		public Boolean uName_Visibility {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return !this.IsNameEditing;
			}
		}

		public String uName_Text {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Name;
			}
		}

		public Boolean uNameEdit_Visibility {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.IsNameEditing;
			}
		}

		public Boolean uNameEdit_IsEnabled {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.IsNameEditing;
			}
		}

		public async void uNameEdit_IsEnabledChanged (
			Object                             sender,
			DependencyPropertyChangedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			if (senders.IsEnabled) {
				senders.Focus(FocusState.Pointer);
			}
			return;
		}

		public async void uNameEdit_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			this.IsNameEditing = false;
			this.NotifyPropertyChanged(
				nameof(this.uName_Visibility),
				nameof(this.uNameEdit_Visibility),
				nameof(this.uNameEdit_IsEnabled)
			);
			var value = senders.Text;
			if (StorageHelper.CheckName(value)) {
				await this.Host.GroupRename(this.Parent.Name, this.Name, value);
			}
			return;
		}

		public Size uCount_Value {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Children.Count;
			}
		}

		// ----------------

		public async void uAction_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			switch (senders.Tag.As<String>()) {
				case "Reload": {
					await this.Host.GroupReload(this.Parent.Name, this.Name);
					break;
				}
				case "RevealRoot": {
					await this.Host.GroupReveal(this.Parent.Name, this.Name, false);
					break;
				}
				case "RevealSetting": {
					await this.Host.GroupReveal(this.Parent.Name, this.Name, true);
					break;
				}
				case "Rename": {
					this.IsNameEditing = true;
					this.NotifyPropertyChanged(
						nameof(this.uName_Text),
						nameof(this.uName_Visibility),
						nameof(this.uNameEdit_Visibility),
						nameof(this.uNameEdit_IsEnabled)
					);
					break;
				}
				case "Delete": {
					await this.Host.GroupDelete(this.Parent.Name, this.Name);
					break;
				}
				case "Copy": {
					await this.Host.GroupCopy(this.Parent.Name, this.Name);
					break;
				}
				case "TranspileCustom": {
					await this.Host.WorkerDoTranspile([$"/{this.Parent.Name}/{this.Name}"], null);
					await this.Host.GroupReload(this.Parent.Name, this.Name);
					break;
				}
				case "TranspileGeneralize": {
					await this.Host.WorkerDoTranspile([$"/{this.Parent.Name}/{this.Name}"], false);
					await this.Host.GroupReload(this.Parent.Name, this.Name);
					break;
				}
				case "TranspileSpecialize": {
					await this.Host.WorkerDoTranspile([$"/{this.Parent.Name}/{this.Name}"], true);
					await this.Host.GroupReload(this.Parent.Name, this.Name);
					break;
				}
				case "Compile": {
					await this.Host.WorkerDoCompile([$"/{this.Parent.Name}/{this.Name}"]);
					break;
				}
				case "Variable": {
					await ControlHelper.ShowDialogAsFixed(this.Host.View, "Group Variable", new VariableListPanel() {
						Value = this.Setting.Variable,
						Stamp = UniqueStamp.Create(),
					}, null);
					await this.SaveSetting();
					break;
				}
				default: throw new ();
			}
			return;
		}

		#endregion

	}

	public class MainPageResourceGroupController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public String PartName { get; set; } = default!;

		public String GroupName { get; set; } = default!;

		// ----------------

		public ObservableCollection<MainPageResourceItemController> Children { get; set; } = default!;

		#endregion

		#region view

		public String uName_Text {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return $"{this.PartName} - {this.GroupName}";
			}
		}

		// ----------------

		public async void uActionAppend_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.ResourceAppend(this.PartName, this.GroupName);
			return;
		}

		#endregion

	}

	public class MainPageResourceItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public String Name { get; set; } = default!;

		public ResourceSetting Setting { get; set; } = default!;

		// ----------------

		public MainPageGroupItemController Parent { get; set; } = default!;

		// ----------------

		public Boolean IsNameEditing { get; set; } = false;

		// ----------------

		public async Task SaveSetting (
		) {
			GF.AssertTest(this.Host.IsLoaded);
			await JsonHelper.SerializeFile(this.Host.MakeScopeSettingPath(this.Host.MakeScopeRootPath(this.Parent.Parent.Name, this.Parent.Name, this.Name)), this.Setting);
			return;
		}

		#endregion

		#region view

		public Boolean uName_Visibility {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return !this.IsNameEditing;
			}
		}

		public String uName_Text {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Name;
			}
		}

		public Boolean uNameEdit_Visibility {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.IsNameEditing;
			}
		}

		public Boolean uNameEdit_IsEnabled {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.IsNameEditing;
			}
		}

		public async void uNameEdit_IsEnabledChanged (
			Object                             sender,
			DependencyPropertyChangedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			if (senders.IsEnabled) {
				senders.Focus(FocusState.Pointer);
			}
			return;
		}

		public async void uNameEdit_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			this.IsNameEditing = false;
			this.NotifyPropertyChanged(
				nameof(this.uName_Visibility),
				nameof(this.uNameEdit_Visibility),
				nameof(this.uNameEdit_IsEnabled)
			);
			var value = senders.Text;
			if (StorageHelper.CheckName(value)) {
				await this.Host.ResourceRename(this.Parent.Parent.Name, this.Parent.Name, this.Name, value);
			}
			return;
		}

		// ----------------

		public Style uCategory_Style {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Host.View.FindResource(this.Setting.Category.Resolution == null && this.Setting.Category.Locale == null ? "DefaultButtonStyle" : "AccentButtonStyle").As<Style>();
			}
		}

		public async void uCategoryFlyout_Closed (
			Object sender,
			Object args
		) {
			var senders = sender.As<FlyoutBase>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.SaveSetting();
			return;
		}

		public async void uCategoryResolution_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			this.Setting.Category.Resolution = ProjectSettingHelper.ParseResolutionString(senders.Text, this.Setting.Category.Resolution);
			this.NotifyPropertyChanged(
				nameof(this.uCategoryResolution_Text),
				nameof(this.uCategory_Style)
			);
			return;
		}

		public String uCategoryResolution_Text {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Setting.Category.Resolution?.ToString() ?? "";
			}
		}

		public async void uCategoryLocale_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			this.Setting.Category.Locale = ProjectSettingHelper.ParseLocaleString(senders.Text, this.Setting.Category.Locale);
			this.NotifyPropertyChanged(
				nameof(this.uCategoryLocale_Text),
				nameof(this.uCategory_Style)
			);
			return;
		}

		public String uCategoryLocale_Text {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Setting.Category.Locale ?? "";
			}
		}

		// ----------------

		public String uTypeIcon_Glyph {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Setting.Type switch {
					ResourceType.Dummy       => FluentIconGlyph.Preview,
					ResourceType.General     => FluentIconGlyph.Document,
					ResourceType.Texture     => FluentIconGlyph.Tiles,
					ResourceType.SpecialRton => FluentIconGlyph.KnowledgeArticle,
					ResourceType.SpecialPtx  => FluentIconGlyph.Picture,
					ResourceType.SpecialPam  => FluentIconGlyph.HomeGroup,
					ResourceType.SpecialWem  => FluentIconGlyph.Audio,
					_                        => throw new (),
				};
			}
		}

		public String uTypeText_Text {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Setting.Type switch {
					ResourceType.Dummy       => "Dummy",
					ResourceType.General     => "General",
					ResourceType.Texture     => "Texture",
					ResourceType.SpecialRton => "RTON",
					ResourceType.SpecialPtx  => "PTX",
					ResourceType.SpecialPam  => "PAM",
					ResourceType.SpecialWem  => "WEM",
					_                        => throw new (),
				};
			}
		}

		public async void uType_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<SplitButton>();
			GF.AssertTest(this.Host.IsLoaded);
			var property = this.Setting.Type switch {
				ResourceType.Dummy       => JsonHelper.DeserializeNode<DummyResourceProperty>(this.Setting.Property) as Object,
				ResourceType.General     => JsonHelper.DeserializeNode<GeneralResourceProperty>(this.Setting.Property),
				ResourceType.Texture     => JsonHelper.DeserializeNode<TextureResourceProperty>(this.Setting.Property),
				ResourceType.SpecialRton => JsonHelper.DeserializeNode<SpecialRtonResourceProperty>(this.Setting.Property),
				ResourceType.SpecialPtx  => JsonHelper.DeserializeNode<SpecialPtxResourceProperty>(this.Setting.Property),
				ResourceType.SpecialPam  => JsonHelper.DeserializeNode<SpecialPamResourceProperty>(this.Setting.Property),
				ResourceType.SpecialWem  => JsonHelper.DeserializeNode<SpecialWemResourceProperty>(this.Setting.Property),
				_                        => throw new (),
			};
			await ControlHelper.ShowDialogAsFixed(this.Host.View, "Resource Property", this.Setting.Type switch {
				ResourceType.Dummy => new DummyResourcePropertyPanel() {
					Value = property.As<DummyResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.General => new GeneralResourcePropertyPanel() {
					Value = property.As<GeneralResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.Texture => new TextureResourcePropertyPanel() {
					Value = property.As<TextureResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.SpecialRton => new SpecialRtonResourcePropertyPanel() {
					ConversionSource = this.Host.ProjectSetting.Package.SelectMany((value) => (value.Conversion.Rton.Select((x) => (x.Name)))).Distinct().ToList(),
					Value = property.As<SpecialRtonResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.SpecialPtx => new SpecialPtxResourcePropertyPanel() {
					ConversionSource = this.Host.ProjectSetting.Package.SelectMany((value) => (value.Conversion.Ptx.Select((x) => (x.Name)))).Distinct().ToList(),
					Value = property.As<SpecialPtxResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.SpecialPam => new SpecialPamResourcePropertyPanel() {
					ConversionSource = this.Host.ProjectSetting.Package.SelectMany((value) => (value.Conversion.Pam.Select((x) => (x.Name)))).Distinct().ToList(),
					Value = property.As<SpecialPamResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.SpecialWem => new SpecialWemResourcePropertyPanel() {
					ConversionSource = this.Host.ProjectSetting.Package.SelectMany((value) => (value.Conversion.Wem.Select((x) => (x.Name)))).Distinct().ToList(),
					Value = property.As<SpecialWemResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				_ => throw new (),
			}, null);
			this.Setting.Property = JsonHelper.SerializeNode(property);
			await this.SaveSetting();
			return;
		}

		public async void uTypeCast_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			var resourceDirectory = this.Host.MakeScopeRootPath(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			StorageHelper.TrashDirectory(resourceDirectory);
			StorageHelper.CreateDirectory(resourceDirectory);
			this.Setting = new () {
				Category = this.Setting.Category,
				Type = default!,
				Property = default!,
				Variable = this.Setting.Variable,
			};
			switch (senders.Tag.As<String>()) {
				case "Dummy": {
					this.Setting.Type = ResourceType.Dummy;
					this.Setting.Property = JsonHelper.SerializeNode(new DummyResourceProperty() {
					});
					break;
				}
				case "General": {
					this.Setting.Type = ResourceType.General;
					this.Setting.Property = JsonHelper.SerializeNode(new GeneralResourceProperty() {
						Path = "",
						Type = "",
					});
					StorageHelper.CreateFile(this.Host.MakeScopeChildPath(resourceDirectory, "source.bin"));
					break;
				}
				case "Texture": {
					this.Setting.Type = ResourceType.Texture;
					this.Setting.Property = JsonHelper.SerializeNode(new TextureResourceProperty() {
						Path = "",
						Format = 0,
						Pitch = 0,
						AdditionalByteCount = 0,
						Size = [0, 0],
						Sprite = [],
					});
					StorageHelper.CreateFile(this.Host.MakeScopeChildPath(resourceDirectory, "source.ptx"));
					break;
				}
				case "SpecialRton": {
					this.Setting.Type = ResourceType.SpecialRton;
					this.Setting.Property = JsonHelper.SerializeNode(new SpecialRtonResourceProperty() {
						Conversion = "",
						Path = "",
					});
					StorageHelper.CreateFile(this.Host.MakeScopeChildPath(resourceDirectory, "source.json"));
					break;
				}
				case "SpecialPtx": {
					this.Setting.Type = ResourceType.SpecialPtx;
					this.Setting.Property = JsonHelper.SerializeNode(new SpecialPtxResourceProperty() {
						Conversion = "",
						Path = "",
						Sprite = [],
					});
					StorageHelper.CreateDirectory(this.Host.MakeScopeChildPath(resourceDirectory, "source.sprite"));
					break;
				}
				case "SpecialPam": {
					this.Setting.Type = ResourceType.SpecialPam;
					this.Setting.Property = JsonHelper.SerializeNode(new SpecialPamResourceProperty() {
						Conversion = "",
						Path = "",
					});
					StorageHelper.CreateFile(this.Host.MakeScopeChildPath(resourceDirectory, "source.json"));
					break;
				}
				case "SpecialWem": {
					this.Setting.Type = ResourceType.SpecialWem;
					this.Setting.Property = JsonHelper.SerializeNode(new SpecialWemResourceProperty() {
						Conversion = "",
						Path = "",
					});
					StorageHelper.CreateFile(this.Host.MakeScopeChildPath(resourceDirectory, "source.wav"));
					break;
				}
				default: throw new ();
			}
			await this.SaveSetting();
			this.NotifyPropertyChanged(
				nameof(this.uTypeIcon_Glyph),
				nameof(this.uTypeText_Text)
			);
			return;
		}

		// ----------------

		public async void uAction_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			switch (senders.Tag.As<String>()) {
				case "Reload": {
					await this.Host.ResourceReload(this.Parent.Parent.Name, this.Parent.Name, this.Name);
					break;
				}
				case "RevealRoot": {
					await this.Host.ResourceReveal(this.Parent.Parent.Name, this.Parent.Name, this.Name, false);
					break;
				}
				case "RevealSetting": {
					await this.Host.ResourceReveal(this.Parent.Parent.Name, this.Parent.Name, this.Name, true);
					break;
				}
				case "Rename": {
					this.IsNameEditing = true;
					this.NotifyPropertyChanged(
						nameof(this.uName_Text),
						nameof(this.uName_Visibility),
						nameof(this.uNameEdit_Visibility),
						nameof(this.uNameEdit_IsEnabled)
					);
					break;
				}
				case "Delete": {
					await this.Host.ResourceDelete(this.Parent.Parent.Name, this.Parent.Name, this.Name);
					break;
				}
				case "Copy": {
					await this.Host.ResourceCopy(this.Parent.Parent.Name, this.Parent.Name, this.Name);
					break;
				}
				case "TranspileCustom": {
					await this.Host.WorkerDoTranspile([$"/{this.Parent.Parent.Name}/{this.Parent.Name}/{this.Name}"], null);
					await this.Host.ResourceReload(this.Parent.Parent.Name, this.Parent.Name, this.Name);
					break;
				}
				case "TranspileGeneralize": {
					await this.Host.WorkerDoTranspile([$"/{this.Parent.Parent.Name}/{this.Parent.Name}/{this.Name}"], false);
					await this.Host.ResourceReload(this.Parent.Parent.Name, this.Parent.Name, this.Name);
					break;
				}
				case "TranspileSpecialize": {
					await this.Host.WorkerDoTranspile([$"/{this.Parent.Parent.Name}/{this.Parent.Name}/{this.Name}"], true);
					await this.Host.ResourceReload(this.Parent.Parent.Name, this.Parent.Name, this.Name);
					break;
				}
				case "Compile": {
					await this.Host.WorkerDoCompile([$"/{this.Parent.Parent.Name}/{this.Parent.Name}/{this.Name}"]);
					break;
				}
				case "Variable": {
					await ControlHelper.ShowDialogAsFixed(this.Host.View, "Resource Variable", new VariableListPanel() {
						Value = this.Setting.Variable,
						Stamp = UniqueStamp.Create(),
					}, null);
					await this.SaveSetting();
					break;
				}
				default: throw new ();
			}
			return;
		}

		#endregion

	}

}
