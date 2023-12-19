#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Helper.CommonControl;
using Microsoft.UI.Xaml.Navigation;
using Newtonsoft.Json.Linq;

namespace Helper.Module.PackageBuilder {

	public sealed partial class MainPage : Page, ITabItemPage {

		#region life

		public MainPage (
		) {
			this.InitializeComponent();
			this.Controller = new MainPageController() { View = this };
			this.Controller.Initialize();
		}

		// ----------------

		protected override void OnNavigatedTo (
			NavigationEventArgs args
		) {
			this.Controller.ApplyOption(args.Parameter.AsClass<List<String>>());
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		private MainPageController Controller { get; }

		#endregion

		#region tab item page

		public async Task<Boolean> TabItemPageRequestClose (
		) {
			return await this.Controller.RequestClose();
		}

		#endregion

	}

	public class MainPageController : CustomController {

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
				return this.ProjectDirectory is not null;
			}
		}

		#endregion

		#region initialize

		public void Initialize (
		) {
			return;
		}

		public async void ApplyOption (
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
					throw new Exception($"Too many option : '{String.Join(' ', option.NextStringList())}'.");
				}
			}
			catch (Exception e) {
				App.MainWindow.PublishTip(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			if (optionProjectDirectory is not null) {
				await this.ProjectOpen(optionProjectDirectory);
			}
			return;
		}

		public async Task<Boolean> RequestClose (
		) {
			return true;
		}

		#endregion

		#region common

		public static String DataViewFormatForGroup = "TwinStar.PackageBuilder.Group";

		public static String DataViewFormatForResource = "TwinStar.PackageBuilder.Resource";

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
			if (result == "s") {
				App.MainWindow.PublishTip(InfoBarSeverity.Success, "Execute succeeded", "");
			}
			else {
				App.MainWindow.PublishTip(InfoBarSeverity.Error, "Execute failed", "");
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
				App.MainWindow.PublishTip(InfoBarSeverity.Error, "Please select single package target", "");
				return;
			}
			await this.WorkerExecuteCommand(ModdingWorker.ForwardHelper.MakeArgumentForCommand(
				null,
				"pvz2.package_project.transpile",
				mode is null
					? new {
						project_directory = this.MakeScopeRootPath(),
						target_package = String.Join('|', this.View.uPackageList.SelectedItems.First().AsClass<MainPagePackageItemController>().Setting.Name),
						target_scope = targetScope is null ? "*" : String.Join('|', targetScope),
					}
					: new {
						project_directory = this.MakeScopeRootPath(),
						target_package = String.Join('|', this.View.uPackageList.SelectedItems.First().AsClass<MainPagePackageItemController>().Setting.Name),
						target_scope = targetScope is null ? "*" : String.Join('|', targetScope),
						option_generalize_rton = !mode,
						option_generalize_ptx = !mode,
						option_generalize_pam = !mode,
						option_generalize_wem = !mode,
						option_specialize_rton = mode,
						option_specialize_ptx = mode,
						option_specialize_pam = mode,
						option_specialize_wem = mode,
					}
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
				new {
					project_directory = this.MakeScopeRootPath(),
					target_package = String.Join('|', this.View.uPackageList.SelectedItems.Select(GF.AsClass<MainPagePackageItemController>).Select((value) => (value.Setting.Name))),
					target_scope = targetScope is null ? "*" : String.Join('|', targetScope),
				}
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
				new {
					project_directory = this.MakeScopeRootPath(),
					target_package = String.Join('|', this.View.uPackageList.SelectedItems.Select(GF.AsClass<MainPagePackageItemController>).Select((value) => (value.Setting.Name))),
					remake_manifest = remakeManifest,
				}
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
			while (Directory.Exists($"{parent}/{nameRequest}")) {
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
			return this.uPartList_ItemsSource.First((value) => (value.Name == part)).AsNotNull();
		}

		public MainPageGroupItemController FindScopeNode (
			String part,
			String group
		) {
			GF.AssertTest(this.IsLoaded);
			return this.FindScopeNode(part).Children.First((value) => (value.Name == group)).AsNotNull();
		}

		public MainPageResourceItemController FindScopeNode (
			String part,
			String group,
			String resource
		) {
			GF.AssertTest(this.IsLoaded);
			return this.FindScopeNode(part, group).Children.First((value) => (value.Name == resource)).AsNotNull();
		}

		// ----------------

		public async Task ProjectOpen (
			String projectDirectory
		) {
			GF.AssertTest(!this.IsLoaded);
			if (!(await this.CheckVersionFile(projectDirectory))) {
				App.MainWindow.PublishTip(InfoBarSeverity.Error, "Failed to check version.txt", "");
				return;
			}
			var dialogClose = ControlHelper.ShowDialogForWaiting(this.View);
			this.ProjectDirectory = projectDirectory;
			this.NotifyPropertyChanged(
				nameof(this.uLoadedAction_IsEnabled),
				nameof(this.uProjectDirectory_Text)
			);
			await App.Instance.AppendRecentJumperItem(new ModuleLauncher.JumperConfiguration() {
				Title = Regex.Replace(StorageHelper.Name(projectDirectory), @"(\.pvz2_package_project)$", "", RegexOptions.IgnoreCase),
				ModuleType = ModuleType.PackageBuilder,
				ModuleOption = new List<String>() {
					"-ProjectDirectory",
					projectDirectory,
				},
				WindowOption = new List<String>(),
			});
			await this.ProjectReload();
			dialogClose();
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
					Children = new ObservableCollection<MainPageGroupItemController>(),
				};
				foreach (var groupName in this.ListScopeChildName(partDirectory)) {
					var groupDirectory = this.MakeScopeChildPath(partDirectory, groupName);
					var groupSetting = await JsonHelper.DeserializeFile<GroupSetting>(this.MakeScopeSettingPath(groupDirectory));
					var groupNode = new MainPageGroupItemController() {
						Host = this,
						Name = groupName,
						Setting = groupSetting,
						Parent = partNode,
						Children = new ObservableCollection<MainPageResourceItemController>(),
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
				Part = new List<String>(),
				Version = new PackageVersion() {
					Number = 4,
					ExtendedTextureInformationForPvz2Cn = 0,
				},
				Compression = new PackageCompression() {
					General = true,
					Texture = true,
					Filter = new List<String>(),
				},
				Manifest = new PackageManifest() {
					Type = PackageManifestType.Internal,
					Suffix = "",
				},
				Category = new PackageCategory() {
					Resolution = new List<Integer>(),
					Locale = new List<String>(),
				},
				Conversion = new PackageConversion() {
					Rton = new List<PackageConversionItem>(),
					Ptx = new List<PackageConversionItem>(),
					Pam = new List<PackageConversionItem>(),
					Wem = new List<PackageConversionItem>(),
				},
				Variable = new List<Variable>(),
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
			var itemNode = this.uPackageList_ItemsSource.First((value) => (Object.ReferenceEquals(value.Setting, sourcePackage))).AsNotNull();
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
			var itemNode = this.uPackageList_ItemsSource.First((value) => (Object.ReferenceEquals(value.Setting, sourcePackage))).AsNotNull();
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
				Version = new PackageVersion() {
					Number = sourcePackage.Version.Number,
					ExtendedTextureInformationForPvz2Cn = sourcePackage.Version.ExtendedTextureInformationForPvz2Cn,
				},
				Compression = new PackageCompression() {
					General = sourcePackage.Compression.General,
					Texture = sourcePackage.Compression.Texture,
					Filter = sourcePackage.Compression.Filter.ToList(),
				},
				Manifest = new PackageManifest() {
					Type = sourcePackage.Manifest.Type,
					Suffix = sourcePackage.Manifest.Suffix,
				},
				Category = new PackageCategory() {
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
				partNode = new MainPagePartItemController() {
					Host = this,
					Name = sourcePart,
					Setting = partSetting,
					Children = new ObservableCollection<MainPageGroupItemController>(),
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
					Children = new ObservableCollection<MainPageResourceItemController>(),
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
				Variable = new List<Variable>(),
			};
			StorageHelper.CreateDirectory(destinationDirectory);
			await JsonHelper.SerializeFile(this.MakeScopeSettingPath(destinationDirectory), destinationSetting);
			var itemNode = new MainPagePartItemController() {
				Host = this,
				Name = destinationPart,
				Setting = destinationSetting,
				Children = new ObservableCollection<MainPageGroupItemController>(),
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
			StorageHelper.RemoveDirectory(sourceDirectory);
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
				groupNode = new MainPageGroupItemController() {
					Host = this,
					Name = sourceGroup,
					Setting = groupSetting,
					Parent = this.FindScopeNode(sourcePart),
					Children = new ObservableCollection<MainPageResourceItemController>(),
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
				Variable = new List<Variable>(),
			};
			StorageHelper.CreateDirectory(destinationDirectory);
			await JsonHelper.SerializeFile(this.MakeScopeSettingPath(destinationDirectory), destinationSetting);
			var parentNode = this.FindScopeNode(sourcePart);
			var itemNode = new MainPageGroupItemController() {
				Host = this,
				Name = destinationGroup,
				Setting = destinationSetting,
				Parent = parentNode,
				Children = new ObservableCollection<MainPageResourceItemController>(),
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
			StorageHelper.RemoveDirectory(sourceDirectory);
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
				resourceNode = new MainPageResourceItemController() {
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
				Category = new ResourceCategory() {
					Resolution = null,
					Locale = null,
				},
				Type = ResourceType.Dummy,
				Property = new JObject(),
				Variable = new List<Variable>(),
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
			StorageHelper.RemoveDirectory(sourceDirectory);
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
			var senders = sender.AsClass<Page>();
			return;
		}

		public async void uPage_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<Page>();
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
				return this.View.FindResource(!this.WorkerState ? "DefaultButtonStyle" : "AccentButtonStyle").AsClass<Style>();
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
			var senders = sender.AsClass<Button>();
			var projectDirectory = await StorageHelper.PickDirectory(App.MainWindow);
			if (projectDirectory is not null) {
				if (this.IsLoaded) {
					await this.ProjectClose();
				}
				await this.ProjectOpen(projectDirectory);
			}
			return;
		}

		public async void uProjectClose_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			GF.AssertTest(this.IsLoaded);
			await this.ProjectClose();
			return;
		}

		// ----------------

		public async void uProjectActionReload_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.ProjectReload();
			return;
		}

		public async void uProjectActionRevealRoot_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.ProjectReveal(false);
			return;
		}

		public async void uProjectActionRevealSetting_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.ProjectReveal(true);
			return;
		}

		public async void uProjectActionTranspileCustom_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(null, null);
			await this.ProjectReload();
			return;
		}

		public async void uProjectActionTranspileGeneralize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(null, false);
			await this.ProjectReload();
			return;
		}

		public async void uProjectActionTranspileSpecialize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(null, true);
			await this.ProjectReload();
			return;
		}

		public async void uProjectActionCompile_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoCompile(null);
			return;
		}

		public async void uProjectActionLinkLegacy_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoLink(false);
			return;
		}

		public async void uProjectActionLinkRemake_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoLink(true);
			return;
		}

		#endregion

		#region package

		public ObservableCollection<MainPagePackageItemController> uPackageList_ItemsSource { get; } = new ObservableCollection<MainPagePackageItemController>();

		public async void uPackageList_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ListView>();
			GF.AssertTest(this.IsLoaded);
			return;
		}

		public async void uPackageList_DragItemsCompleted (
			Object                      sender,
			DragItemsCompletedEventArgs args
		) {
			var senders = sender.AsClass<ListView>();
			GF.AssertTest(this.IsLoaded);
			this.ProjectSetting.Package = this.uPackageList_ItemsSource.Select((value) => (value.Setting)).ToList();
			await this.ProjectSaveSetting();
			return;
		}

		// ----------------

		public async void uPackageActionAppend_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			GF.AssertTest(this.IsLoaded);
			await this.PackageAppend();
			return;
		}

		public async void uPackageActionDelete_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uPackageList.SelectedItems.Select(GF.AsClass<MainPagePackageItemController>).ToList()) {
				await this.PackageDelete(item.Setting);
			}
			return;
		}

		public async void uPackageActionCopy_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uPackageList.SelectedItems.Select(GF.AsClass<MainPagePackageItemController>).ToList()) {
				await this.PackageCopy(item.Setting);
			}
			return;
		}

		#endregion

		#region part

		public ObservableCollection<MainPagePartItemController> uPartList_ItemsSource { get; } = new ObservableCollection<MainPagePartItemController>();

		public async void uPartList_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ListView>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in args.RemovedItems.Select(GF.AsClass<MainPagePartItemController>)) {
				this.uGroupList_ItemsSource.Remove(this.uGroupList_ItemsSource.First((value) => (value.PartName == item.Name)));
			}
			foreach (var item in args.AddedItems.Select(GF.AsClass<MainPagePartItemController>)) {
				this.uGroupList_ItemsSource.Add(new MainPageGroupGroupController() {
					Host = this,
					PartName = item.Name,
					Children = item.Children,
				});
			}
			return;
		}

		// ----------------

		public async void uPartActionReload_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).ToList()) {
				await this.PartReload(item.Name);
			}
			return;
		}

		public async void uPartActionRevealRoot_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).ToList()) {
				await this.PartReveal(item.Name, false);
			}
			return;
		}

		public async void uPartActionRevealSetting_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).ToList()) {
				await this.PartReveal(item.Name, true);
			}
			return;
		}

		public async void uPartActionAppend_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			GF.AssertTest(this.IsLoaded);
			await this.PartAppend();
			return;
		}

		public async void uPartActionDelete_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).ToList()) {
				await this.PartDelete(item.Name);
			}
			return;
		}

		public async void uPartActionCopy_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).ToList()) {
				await this.PartCopy(item.Name);
			}
			return;
		}

		public async void uPartActionTranspileCustom_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).Select((value) => ($"/{value.Name}")).ToList(), null);
			foreach (var item in this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).ToList()) {
				await this.PartReload(item.Name);
			}
			return;
		}

		public async void uPartActionTranspileGeneralize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).Select((value) => ($"/{value.Name}")).ToList(), false);
			foreach (var item in this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).ToList()) {
				await this.PartReload(item.Name);
			}
			return;
		}

		public async void uPartActionTranspileSpecialize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).Select((value) => ($"/{value.Name}")).ToList(), true);
			foreach (var item in this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).ToList()) {
				await this.PartReload(item.Name);
			}
			return;
		}

		public async void uPartActionCompile_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoCompile(this.View.uPartList.SelectedItems.Select(GF.AsClass<MainPagePartItemController>).Select((value) => ($"/{value.Name}")).ToList());
			return;
		}

		#endregion

		#region group

		public ObservableCollection<MainPageGroupGroupController> uGroupList_ItemsSource { get; } = new ObservableCollection<MainPageGroupGroupController>();

		public async void uGroupList_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ListView>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in args.RemovedItems.Select(GF.AsClass<MainPageGroupItemController>)) {
				this.uResourceList_ItemsSource.Remove(this.uResourceList_ItemsSource.First((value) => (value.PartName == item.Parent.Name && value.GroupName == item.Name)));
			}
			foreach (var item in args.AddedItems.Select(GF.AsClass<MainPageGroupItemController>)) {
				this.uResourceList_ItemsSource.Add(new MainPageResourceGroupController() {
					Host = this,
					PartName = item.Parent.Name,
					GroupName = item.Name,
					Children = item.Children,
				});
			}
			return;
		}

		public async void uGroupList_DragItemsStarting (
			Object                     sender,
			DragItemsStartingEventArgs args
		) {
			var senders = sender.AsClass<ListView>();
			GF.AssertTest(this.IsLoaded);
			args.Data.SetData(MainPageController.DataViewFormatForGroup, String.Join("\n", args.Items.Select(GF.AsClass<MainPageGroupItemController>).Select((value) => ($"{value.Parent.Name}/{value.Name}"))));
			return;
		}

		// ----------------

		public async void uGroupActionReload_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).ToList()) {
				await this.GroupReload(item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uGroupActionRevealRoot_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).ToList()) {
				await this.GroupReveal(item.Parent.Name, item.Name, false);
			}
			return;
		}

		public async void uGroupActionRevealSetting_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).ToList()) {
				await this.GroupReveal(item.Parent.Name, item.Name, true);
			}
			return;
		}

		public async void uGroupActionDelete_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).ToList()) {
				await this.GroupDelete(item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uGroupActionCopy_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).ToList()) {
				await this.GroupCopy(item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uGroupActionTranspileCustom_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).Select((value) => ($"/{value.Parent.Name}/{value.Name}")).ToList(), null);
			foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).ToList()) {
				await this.GroupReload(item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uGroupActionTranspileGeneralize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).Select((value) => ($"/{value.Parent.Name}/{value.Name}")).ToList(), false);
			foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).ToList()) {
				await this.GroupReload(item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uGroupActionTranspileSpecialize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).Select((value) => ($"/{value.Parent.Name}/{value.Name}")).ToList(), true);
			foreach (var item in this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).ToList()) {
				await this.GroupReload(item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uGroupActionCompile_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoCompile(this.View.uGroupList.SelectedItems.Select(GF.AsClass<MainPageGroupItemController>).Select((value) => ($"/{value.Parent.Name}/{value.Name}")).ToList());
			return;
		}

		#endregion

		#region resource

		public ObservableCollection<MainPageResourceGroupController> uResourceList_ItemsSource { get; } = new ObservableCollection<MainPageResourceGroupController>();

		public async void uResourceList_DragItemsStarting (
			Object                     sender,
			DragItemsStartingEventArgs args
		) {
			var senders = sender.AsClass<ListView>();
			GF.AssertTest(this.IsLoaded);
			args.Data.SetData(MainPageController.DataViewFormatForResource, String.Join("\n", args.Items.Select(GF.AsClass<MainPageResourceItemController>).Select((value) => ($"{value.Parent.Parent.Name}/{value.Parent.Name}/{value.Name}"))));
			return;
		}

		// ----------------

		public async void uResourceActionReload_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).ToList()) {
				await this.ResourceReload(item.Parent.Parent.Name, item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uResourceActionRevealRoot_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).ToList()) {
				await this.ResourceReveal(item.Parent.Parent.Name, item.Parent.Name, item.Name, false);
			}
			return;
		}

		public async void uResourceActionRevealSetting_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).ToList()) {
				await this.ResourceReveal(item.Parent.Parent.Name, item.Parent.Name, item.Name, true);
			}
			return;
		}

		public async void uResourceActionDelete_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).ToList()) {
				await this.ResourceDelete(item.Parent.Parent.Name, item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uResourceActionCopy_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).ToList()) {
				await this.ResourceCopy(item.Parent.Parent.Name, item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uResourceActionTranspileCustom_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).Select((value) => ($"/{value.Parent.Parent.Name}/{value.Parent.Name}/{value.Name}")).ToList(), null);
			foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).ToList()) {
				await this.ResourceReload(item.Parent.Parent.Name, item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uResourceActionTranspileGeneralize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).Select((value) => ($"/{value.Parent.Parent.Name}/{value.Parent.Name}/{value.Name}")).ToList(), false);
			foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).ToList()) {
				await this.ResourceReload(item.Parent.Parent.Name, item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uResourceActionTranspileSpecialize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoTranspile(this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).Select((value) => ($"/{value.Parent.Parent.Name}/{value.Parent.Name}/{value.Name}")).ToList(), true);
			foreach (var item in this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).ToList()) {
				await this.ResourceReload(item.Parent.Parent.Name, item.Parent.Name, item.Name);
			}
			return;
		}

		public async void uResourceActionCompile_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.IsLoaded);
			await this.WorkerDoCompile(this.View.uResourceList.SelectedItems.Select(GF.AsClass<MainPageResourceItemController>).Select((value) => ($"/{value.Parent.Parent.Name}/{value.Parent.Name}/{value.Name}")).ToList());
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
			var senders = sender.AsClass<TextBox>();
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
			var senders = sender.AsClass<TextBox>();
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

		public async void uActionRename_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			this.IsNameEditing = true;
			this.NotifyPropertyChanged(
				nameof(this.uName_Text),
				nameof(this.uName_Visibility),
				nameof(this.uNameEdit_Visibility),
				nameof(this.uNameEdit_IsEnabled)
			);
			return;
		}

		public async void uActionDelete_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.PackageDelete(this.Setting);
			return;
		}

		public async void uActionCopy_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.PackageCopy(this.Setting);
			return;
		}

		public async void uActionPart_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			var partList = this.Host.uPartList_ItemsSource.Select((value) => (value.Name)).ToList();
			await ControlHelper.ShowDialogFixedSize(this.Host.View, "Package Part", new PackagePartPanel() {
				Source = partList,
				Value = this.Setting.Part,
				Stamp = UniqueStamp.Create(),
			});
			this.Setting.Part = partList.Where((value) => (this.Setting.Part.Contains(value))).ToList();
			await this.SaveSetting();
			this.NotifyPropertyChanged(
				nameof(this.uCount_Value)
			);
			return;
		}

		public async void uActionSetting_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await ControlHelper.ShowDialogFixedSize(this.Host.View, "Package Setting", new PackageSettingPanel() {
				Value = this.Setting,
				Stamp = UniqueStamp.Create(),
			});
			await this.SaveSetting();
			return;
		}

		public async void uActionVariable_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await ControlHelper.ShowDialogFixedSize(this.Host.View, "Package Variable", new VariableListPanel() {
				Value = this.Setting.Variable,
				Stamp = UniqueStamp.Create(),
			});
			await this.SaveSetting();
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
			var senders = sender.AsClass<Stack>();
			GF.AssertTest(this.Host.IsLoaded);
			if (args.DataView.Contains(MainPageController.DataViewFormatForGroup)) {
				args.AcceptedOperation = Windows.ApplicationModel.DataTransfer.DataPackageOperation.Move;
			}
			return;
		}

		public async void uRoot_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<Stack>();
			GF.AssertTest(this.Host.IsLoaded);
			if (args.DataView.Contains(MainPageController.DataViewFormatForGroup)) {
				args.Handled = true;
				var data = (await args.DataView.GetDataAsync(MainPageController.DataViewFormatForGroup)).AsClass<String>();
				if (data.Length != 0) {
					var itemList = data.Split("\n");
					foreach (var item in itemList) {
						var scope = item.Split("/");
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
			var senders = sender.AsClass<TextBox>();
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
			var senders = sender.AsClass<TextBox>();
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

		public async void uActionReload_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.PartReload(this.Name);
			return;
		}

		public async void uActionRevealRoot_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.PartReveal(this.Name, false);
			return;
		}

		public async void uActionRevealSetting_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.PartReveal(this.Name, true);
			return;
		}

		public async void uActionRename_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			this.IsNameEditing = true;
			this.NotifyPropertyChanged(
				nameof(this.uName_Text),
				nameof(this.uName_Visibility),
				nameof(this.uNameEdit_Visibility),
				nameof(this.uNameEdit_IsEnabled)
			);
			return;
		}

		public async void uActionDelete_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.PartDelete(this.Name);
			return;
		}

		public async void uActionCopy_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.PartCopy(this.Name);
			return;
		}

		public async void uActionTranspileCustom_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoTranspile(new List<String>() { $"/{this.Name}" }, null);
			await this.Host.PartReload(this.Name);
			return;
		}

		public async void uActionTranspileGeneralize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoTranspile(new List<String>() { $"/{this.Name}" }, false);
			await this.Host.PartReload(this.Name);
			return;
		}

		public async void uActionTranspileSpecialize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoTranspile(new List<String>() { $"/{this.Name}" }, true);
			await this.Host.PartReload(this.Name);
			return;
		}

		public async void uActionCompile_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoCompile(new List<String>() { $"/{this.Name}" });
			return;
		}

		public async void uActionVariable_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await ControlHelper.ShowDialogFixedSize(this.Host.View, "Part Variable", new VariableListPanel() {
				Value = this.Setting.Variable,
				Stamp = UniqueStamp.Create(),
			});
			await this.SaveSetting();
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
			var senders = sender.AsClass<Button>();
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
			var senders = sender.AsClass<Stack>();
			GF.AssertTest(this.Host.IsLoaded);
			if (args.DataView.Contains(MainPageController.DataViewFormatForResource)) {
				args.AcceptedOperation = Windows.ApplicationModel.DataTransfer.DataPackageOperation.Move;
			}
			return;
		}

		public async void uRoot_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<Stack>();
			GF.AssertTest(this.Host.IsLoaded);
			if (args.DataView.Contains(MainPageController.DataViewFormatForResource)) {
				args.Handled = true;
				var data = (await args.DataView.GetDataAsync(MainPageController.DataViewFormatForResource)).AsClass<String>();
				if (data.Length != 0) {
					var itemList = data.Split("\n");
					foreach (var item in itemList) {
						var scope = item.Split("/");
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
			var senders = sender.AsClass<TextBox>();
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
			var senders = sender.AsClass<TextBox>();
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

		public async void uActionReload_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.GroupReload(this.Parent.Name, this.Name);
			return;
		}

		public async void uActionRevealRoot_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.GroupReveal(this.Parent.Name, this.Name, false);
			return;
		}

		public async void uActionRevealSetting_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.GroupReveal(this.Parent.Name, this.Name, true);
			return;
		}

		public async void uActionRename_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			this.IsNameEditing = true;
			this.NotifyPropertyChanged(
				nameof(this.uName_Text),
				nameof(this.uName_Visibility),
				nameof(this.uNameEdit_Visibility),
				nameof(this.uNameEdit_IsEnabled)
			);
			return;
		}

		public async void uActionDelete_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.GroupDelete(this.Parent.Name, this.Name);
			return;
		}

		public async void uActionCopy_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.GroupCopy(this.Parent.Name, this.Name);
			return;
		}

		public async void uActionTranspileCustom_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoTranspile(new List<String>() { $"/{this.Parent.Name}/{this.Name}" }, null);
			await this.Host.GroupReload(this.Parent.Name, this.Name);
			return;
		}

		public async void uActionTranspileGeneralize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoTranspile(new List<String>() { $"/{this.Parent.Name}/{this.Name}" }, false);
			await this.Host.GroupReload(this.Parent.Name, this.Name);
			return;
		}

		public async void uActionTranspileSpecialize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoTranspile(new List<String>() { $"/{this.Parent.Name}/{this.Name}" }, true);
			await this.Host.GroupReload(this.Parent.Name, this.Name);
			return;
		}

		public async void uActionCompile_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoCompile(new List<String>() { $"/{this.Parent.Name}/{this.Name}" });
			return;
		}

		public async void uActionVariable_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await ControlHelper.ShowDialogFixedSize(this.Host.View, "Group Variable", new VariableListPanel() {
				Value = this.Setting.Variable,
				Stamp = UniqueStamp.Create(),
			});
			await this.SaveSetting();
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
			var senders = sender.AsClass<Button>();
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
			var senders = sender.AsClass<TextBox>();
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
			var senders = sender.AsClass<TextBox>();
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
				return this.Host.View.FindResource(this.Setting.Category is { Resolution: null, Locale: null } ? "DefaultButtonStyle" : "AccentButtonStyle").AsClass<Style>();
			}
		}

		public async void uCategoryFlyout_Closed (
			Object sender,
			Object args
		) {
			var senders = sender.AsClass<FlyoutBase>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.SaveSetting();
			return;
		}

		public String uCategoryResolution_Text {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Setting.Category.Resolution?.ToString() ?? "";
			}
		}

		public async void uCategoryResolution_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			this.Setting.Category.Resolution = ProjectSettingHelper.ParseResolutionString(senders.Text);
			this.NotifyPropertyChanged(
				nameof(this.uCategory_Style),
				nameof(this.uCategoryResolution_Text)
			);
			return;
		}

		public String uCategoryLocale_Text {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Setting.Category.Locale ?? "";
			}
		}

		public async void uCategoryLocale_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<TextBox>();
			GF.AssertTest(this.Host.IsLoaded);
			this.Setting.Category.Locale = ProjectSettingHelper.ParseLocaleString(senders.Text);
			this.NotifyPropertyChanged(
				nameof(this.uCategory_Style),
				nameof(this.uCategoryLocale_Text)
			);
			return;
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
					_                        => throw new ArgumentOutOfRangeException(),
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
					_                        => throw new ArgumentOutOfRangeException(),
				};
			}
		}

		public async void uType_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.AsClass<SplitButton>();
			GF.AssertTest(this.Host.IsLoaded);
			var property = this.Setting.Type switch {
				ResourceType.Dummy       => JsonHelper.DeserializeToken<DummyResourceProperty>(this.Setting.Property) as Object,
				ResourceType.General     => JsonHelper.DeserializeToken<GeneralResourceProperty>(this.Setting.Property),
				ResourceType.Texture     => JsonHelper.DeserializeToken<TextureResourceProperty>(this.Setting.Property),
				ResourceType.SpecialRton => JsonHelper.DeserializeToken<SpecialRtonResourceProperty>(this.Setting.Property),
				ResourceType.SpecialPtx  => JsonHelper.DeserializeToken<SpecialPtxResourceProperty>(this.Setting.Property),
				ResourceType.SpecialPam  => JsonHelper.DeserializeToken<SpecialPamResourceProperty>(this.Setting.Property),
				ResourceType.SpecialWem  => JsonHelper.DeserializeToken<SpecialWemResourceProperty>(this.Setting.Property),
				_                        => throw new ArgumentOutOfRangeException(),
			};
			await ControlHelper.ShowDialogFixedSize(this.Host.View, "Resource Property", this.Setting.Type switch {
				ResourceType.Dummy => new DummyResourcePropertyPanel() {
					Value = property.AsClass<DummyResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.General => new GeneralResourcePropertyPanel() {
					Value = property.AsClass<GeneralResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.Texture => new TextureResourcePropertyPanel() {
					Value = property.AsClass<TextureResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.SpecialRton => new SpecialRtonResourcePropertyPanel() {
					ConversionSource = this.Host.ProjectSetting.Package.SelectMany((value) => (value.Conversion.Rton.Select((x) => (x.Name)))).Distinct().ToList(),
					Value = property.AsClass<SpecialRtonResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.SpecialPtx => new SpecialPtxResourcePropertyPanel() {
					ConversionSource = this.Host.ProjectSetting.Package.SelectMany((value) => (value.Conversion.Ptx.Select((x) => (x.Name)))).Distinct().ToList(),
					Value = property.AsClass<SpecialPtxResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.SpecialPam => new SpecialPamResourcePropertyPanel() {
					ConversionSource = this.Host.ProjectSetting.Package.SelectMany((value) => (value.Conversion.Pam.Select((x) => (x.Name)))).Distinct().ToList(),
					Value = property.AsClass<SpecialPamResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				ResourceType.SpecialWem => new SpecialWemResourcePropertyPanel() {
					ConversionSource = this.Host.ProjectSetting.Package.SelectMany((value) => (value.Conversion.Wem.Select((x) => (x.Name)))).Distinct().ToList(),
					Value = property.AsClass<SpecialWemResourceProperty>(),
					Stamp = UniqueStamp.Create(),
				},
				_ => throw new ArgumentOutOfRangeException(),
			});
			this.Setting.Property = JsonHelper.SerializeToken(property);
			await this.SaveSetting();
			return;
		}

		public async void uTypeCastDummy_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			this.Setting = new ResourceSetting() {
				Category = this.Setting.Category,
				Type = ResourceType.Dummy,
				Property = JsonHelper.SerializeToken(new DummyResourceProperty() {
				}),
				Variable = this.Setting.Variable,
			};
			var resourceDirectory = this.Host.MakeScopeRootPath(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			StorageHelper.RemoveDirectory(resourceDirectory);
			StorageHelper.CreateDirectory(resourceDirectory);
			await this.SaveSetting();
			this.NotifyPropertyChanged(
				nameof(this.uTypeIcon_Glyph),
				nameof(this.uTypeText_Text)
			);
			return;
		}

		public async void uTypeCastGeneral_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			this.Setting = new ResourceSetting() {
				Category = this.Setting.Category,
				Type = ResourceType.General,
				Property = JsonHelper.SerializeToken(new GeneralResourceProperty() {
					Path = "",
					Type = "",
				}),
				Variable = this.Setting.Variable,
			};
			var resourceDirectory = this.Host.MakeScopeRootPath(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			StorageHelper.RemoveDirectory(resourceDirectory);
			StorageHelper.CreateDirectory(resourceDirectory);
			await this.SaveSetting();
			StorageHelper.CreateFile(this.Host.MakeScopeChildPath(resourceDirectory, "source.bin"));
			this.NotifyPropertyChanged(
				nameof(this.uTypeIcon_Glyph),
				nameof(this.uTypeText_Text)
			);
			return;
		}

		public async void uTypeCastTexture_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			this.Setting = new ResourceSetting() {
				Category = this.Setting.Category,
				Type = ResourceType.Texture,
				Property = JsonHelper.SerializeToken(new TextureResourceProperty() {
					Path = "",
					Format = 0,
					Pitch = 0,
					AdditionalByteCount = 0,
					Size = new List<Integer>() { 0, 0 },
					Sprite = new List<TextureResourcePropertySpriteResource>(),
				}),
				Variable = this.Setting.Variable,
			};
			var resourceDirectory = this.Host.MakeScopeRootPath(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			StorageHelper.RemoveDirectory(resourceDirectory);
			StorageHelper.CreateDirectory(resourceDirectory);
			await this.SaveSetting();
			StorageHelper.CreateFile(this.Host.MakeScopeChildPath(resourceDirectory, "source.ptx"));
			this.NotifyPropertyChanged(
				nameof(this.uTypeIcon_Glyph),
				nameof(this.uTypeText_Text)
			);
			return;
		}

		public async void uTypeCastSpecialRton_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			this.Setting = new ResourceSetting() {
				Category = this.Setting.Category,
				Type = ResourceType.SpecialRton,
				Property = JsonHelper.SerializeToken(new SpecialRtonResourceProperty() {
					Conversion = "",
					Path = "",
				}),
				Variable = this.Setting.Variable,
			};
			var resourceDirectory = this.Host.MakeScopeRootPath(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			StorageHelper.RemoveDirectory(resourceDirectory);
			StorageHelper.CreateDirectory(resourceDirectory);
			await this.SaveSetting();
			StorageHelper.CreateFile(this.Host.MakeScopeChildPath(resourceDirectory, "source.json"));
			this.NotifyPropertyChanged(
				nameof(this.uTypeIcon_Glyph),
				nameof(this.uTypeText_Text)
			);
			return;
		}

		public async void uTypeCastSpecialPtx_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			this.Setting = new ResourceSetting() {
				Category = this.Setting.Category,
				Type = ResourceType.SpecialPtx,
				Property = JsonHelper.SerializeToken(new SpecialPtxResourceProperty() {
					Conversion = "",
					Path = "",
					Sprite = new List<SpecialPtxResourcePropertySpriteResource>(),
				}),
				Variable = this.Setting.Variable,
			};
			var resourceDirectory = this.Host.MakeScopeRootPath(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			StorageHelper.RemoveDirectory(resourceDirectory);
			StorageHelper.CreateDirectory(resourceDirectory);
			await this.SaveSetting();
			StorageHelper.CreateDirectory(this.Host.MakeScopeChildPath(resourceDirectory, "source.sprite"));
			this.NotifyPropertyChanged(
				nameof(this.uTypeIcon_Glyph),
				nameof(this.uTypeText_Text)
			);
			return;
		}

		public async void uTypeCastSpecialPam_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			this.Setting = new ResourceSetting() {
				Category = this.Setting.Category,
				Type = ResourceType.SpecialPam,
				Property = JsonHelper.SerializeToken(new SpecialPamResourceProperty() {
					Conversion = "",
					Path = "",
				}),
				Variable = this.Setting.Variable,
			};
			var resourceDirectory = this.Host.MakeScopeRootPath(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			StorageHelper.RemoveDirectory(resourceDirectory);
			StorageHelper.CreateDirectory(resourceDirectory);
			await this.SaveSetting();
			StorageHelper.CreateFile(this.Host.MakeScopeChildPath(resourceDirectory, "source.json"));
			this.NotifyPropertyChanged(
				nameof(this.uTypeIcon_Glyph),
				nameof(this.uTypeText_Text)
			);
			return;
		}

		public async void uTypeCastSpecialWem_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			this.Setting = new ResourceSetting() {
				Category = this.Setting.Category,
				Type = ResourceType.SpecialWem,
				Property = JsonHelper.SerializeToken(new SpecialWemResourceProperty() {
					Conversion = "",
					Path = "",
				}),
				Variable = this.Setting.Variable,
			};
			var resourceDirectory = this.Host.MakeScopeRootPath(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			StorageHelper.RemoveDirectory(resourceDirectory);
			StorageHelper.CreateDirectory(resourceDirectory);
			await this.SaveSetting();
			StorageHelper.CreateFile(this.Host.MakeScopeChildPath(resourceDirectory, "source.wav"));
			this.NotifyPropertyChanged(
				nameof(this.uTypeIcon_Glyph),
				nameof(this.uTypeText_Text)
			);
			return;
		}

		// ----------------

		public async void uActionReload_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.ResourceReload(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			return;
		}

		public async void uActionRevealRoot_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.ResourceReveal(this.Parent.Parent.Name, this.Parent.Name, this.Name, false);
			return;
		}

		public async void uActionRevealSetting_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.ResourceReveal(this.Parent.Parent.Name, this.Parent.Name, this.Name, true);
			return;
		}

		public async void uActionRename_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			this.IsNameEditing = true;
			this.NotifyPropertyChanged(
				nameof(this.uName_Text),
				nameof(this.uName_Visibility),
				nameof(this.uNameEdit_Visibility),
				nameof(this.uNameEdit_IsEnabled)
			);
			return;
		}

		public async void uActionDelete_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.ResourceDelete(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			return;
		}

		public async void uActionCopy_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.ResourceCopy(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			return;
		}

		public async void uActionTranspileCustom_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoTranspile(new List<String>() { $"/{this.Parent.Parent.Name}/{this.Parent.Name}/{this.Name}" }, null);
			await this.Host.ResourceReload(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			return;
		}

		public async void uActionTranspileGeneralize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoTranspile(new List<String>() { $"/{this.Parent.Parent.Name}/{this.Parent.Name}/{this.Name}" }, false);
			await this.Host.ResourceReload(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			return;
		}

		public async void uActionTranspileSpecialize_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoTranspile(new List<String>() { $"/{this.Parent.Parent.Name}/{this.Parent.Name}/{this.Name}" }, true);
			await this.Host.ResourceReload(this.Parent.Parent.Name, this.Parent.Name, this.Name);
			return;
		}

		public async void uActionCompile_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await this.Host.WorkerDoCompile(new List<String>() { $"/{this.Parent.Parent.Name}/{this.Parent.Name}/{this.Name}" });
			return;
		}

		public async void uActionVariable_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<MenuFlyoutItem>();
			GF.AssertTest(this.Host.IsLoaded);
			await ControlHelper.ShowDialogFixedSize(this.Host.View, "Resource Variable", new VariableListPanel() {
				Value = this.Setting.Variable,
				Stamp = UniqueStamp.Create(),
			});
			await this.SaveSetting();
			return;
		}

		#endregion

	}

}
