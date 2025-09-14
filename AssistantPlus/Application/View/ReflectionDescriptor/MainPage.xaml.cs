#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel.DataTransfer;

namespace AssistantPlus.View.ReflectionDescriptor {

	public sealed partial class MainPage : Page, Home.IModulePage {

		#region life

		private MainPageController Controller { get; }

		// ----------------

		public MainPage (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			this.Controller.InitializeView();
			ControlHelper.PostTask(this, async () => {
				await this.Controller.OpenView();
				await this.Controller.ApplyOption(this.Tag.As<List<String>>());
			}).SelfLet(ExceptionHelper.WrapTask);
			return;
		}

		#endregion

		#region module page

		public Home.IModulePageController ModulePageGetController (
		) {
			return this.Controller;
		}

		#endregion

	}

	public partial class MainPageController : CustomController, Home.IModulePageController {

		#region data

		public MainPage View { get; init; } = default!;

		// ----------------

		public String? DescriptorFile { get; set; } = null;

		public GameReflectionModel.DescriptorArchive? DescriptorArchive { get; set; } = null;

		public GameReflectionModel.DescriptorMap? DescriptorMap { get; set; } = null;

		// ----------------

		[MemberNotNullWhen(true, nameof(MainPageController.DescriptorFile))]
		[MemberNotNullWhen(true, nameof(MainPageController.DescriptorArchive))]
		[MemberNotNullWhen(true, nameof(MainPageController.DescriptorMap))]
		public Boolean IsLoaded {
			get {
				return this.DescriptorFile != null
					&& this.DescriptorArchive != null
					&& this.DescriptorMap != null;
			}
		}

		#endregion

		#region life

		public void InitializeView (
		) {
			return;
		}

		public async Task OpenView (
		) {
			return;
		}

		public async Task<Boolean> CloseView (
		) {
			return true;
		}

		public async Task EnterView (
		) {
			return;
		}

		public async Task ExitView (
		) {
			return;
		}

		public async Task ApplyOption (
			List<String> optionView
		) {
			var optionDescriptorFile = default(String?);
			var option = new CommandLineReader(optionView);
			if (option.Check("-DescriptorFile")) {
				optionDescriptorFile = option.NextString();
			}
			if (!option.Done()) {
				throw new ($"Too many option '{String.Join(' ', option.NextStringList())}'.");
			}
			if (optionDescriptorFile != null) {
				await this.ApplyLoad(optionDescriptorFile);
			}
			return;
		}

		public async Task<List<String>> CollectOption (
		) {
			var option = new CommandLineWriter();
			if (option.Check("-DescriptorFile", this.DescriptorFile != null)) {
				option.NextString(this.DescriptorFile.AsNotNull());
			}
			return option.Done();
		}

		#endregion

		#region action

		public ObjectItemController CreateSource (
			GameReflectionModel.ObjectDescriptor descriptor
		) {
			return new () {
				Host = this,
				Descriptor = descriptor,
				Children = descriptor.Derived.Select(this.CreateSource).ToList(),
			};
		}

		// ----------------

		public async Task ApplyLoad (
			String descriptorFile
		) {
			if (this.IsLoaded) {
				await this.Unload();
			}
			await this.Load(descriptorFile);
			if (this.IsLoaded) {
				await App.Instance.AppendRecentLauncherItem(new () {
					Title = Regex.Replace(StorageHelper.Name(descriptorFile), @"(\.json)$", "", RegexOptions.IgnoreCase),
					Type = ModuleType.ReflectionDescriptor,
					Option = await this.CollectOption(),
					Command = [],
				});
			}
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
					await App.MainWindow.PushNotification(InfoBarSeverity.Error, "Source is multiply.", "");
					return;
				}
				var descriptorFile = StorageHelper.GetLongPath(item[0].Path);
				if (!StorageHelper.ExistFile(descriptorFile)) {
					await App.MainWindow.PushNotification(InfoBarSeverity.Error, "Source is not a file.", "");
					return;
				}
				await this.ApplyLoad(descriptorFile);
			}
			return;
		}

		#endregion

		#region load

		public async Task Load (
			String descriptorFile
		) {
			GF.AssertTest(!this.IsLoaded);
			var descriptorArchive = await GameReflectionHelper.LoadDescriptorArchive(descriptorFile);
			var descriptorMap = GameReflectionHelper.CompileDescriptorArchive(descriptorArchive);
			this.DescriptorFile = descriptorFile;
			this.DescriptorArchive = descriptorArchive;
			this.DescriptorMap = descriptorMap;
			this.uObjectList_ItemsSource = this.DescriptorArchive.Object.Select(this.CreateSource).ToList();
			this.uObjectList_SelectedItem = this.uObjectList_ItemsSource.FirstOrDefault();
			this.NotifyPropertyChanged([
				nameof(this.uDescriptorFile_Content),
				nameof(this.uDescriptorFileMenuClear_IsEnabled),
				nameof(this.uObjectList_ItemsSource),
				nameof(this.uObjectList_SelectedItem),
				nameof(this.uPropertyExport_IsEnabled),
				nameof(this.uPropertyPanel_Descriptor),
				nameof(this.uPropertyPanel_Type),
				nameof(this.uPropertyPanel_Stamp),
			]);
			return;
		}

		public async Task Unload (
		) {
			GF.AssertTest(this.IsLoaded);
			this.DescriptorFile = null;
			this.DescriptorArchive = null;
			this.DescriptorMap = null;
			this.uObjectList_ItemsSource = [];
			this.uObjectList_SelectedItem = null;
			this.NotifyPropertyChanged([
				nameof(this.uDescriptorFile_Content),
				nameof(this.uDescriptorFileMenuClear_IsEnabled),
				nameof(this.uObjectList_ItemsSource),
				nameof(this.uObjectList_SelectedItem),
				nameof(this.uPropertyExport_IsEnabled),
				nameof(this.uPropertyPanel_Descriptor),
				nameof(this.uPropertyPanel_Type),
				nameof(this.uPropertyPanel_Stamp),
			]);
			return;
		}

		#endregion

		#region source

		public String uDescriptorFile_Content {
			get {
				if (!this.IsLoaded) {
					return "None";
				}
				return new Regex(@"(\.json)$", RegexOptions.IgnoreCase).Replace(StorageHelper.Name(this.DescriptorFile), "");
			}
		}

		// ----------------

		public Boolean uDescriptorFileMenuClear_IsEnabled {
			get {
				if (!this.IsLoaded) {
					return false;
				}
				return true;
			}
		}

		public async void uDescriptorFileMenu_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<MenuFlyoutItem>();
			switch (senders.Tag.As<String>()) {
				case "Load": {
					var file = await StorageHelper.PickLoadFile(App.MainWindow, $"@{nameof(ReflectionDescriptor)}.DescriptorFile");
					if (file != null) {
						await this.ApplyLoad(file);
					}
					break;
				}
				case "Clear": {
					GF.AssertTest(this.IsLoaded);
					await this.Unload();
					break;
				}
				default: throw new UnreachableException();
			}
			return;
		}

		#endregion

		#region object list

		public List<ObjectItemController> uObjectList_ItemsSource { get; set; } = [];

		public ObjectItemController? uObjectList_SelectedItem { get; set; } = null;

		public async void uObjectList_ItemInvoked (
			TreeView                     sender,
			TreeViewItemInvokedEventArgs args
		) {
			var senders = sender.As<TreeView>();
			GF.AssertTest(this.IsLoaded);
			this.uObjectList_SelectedItem = args.InvokedItem.As<ObjectItemController>();
			this.NotifyPropertyChanged([
				nameof(this.uPropertyExport_IsEnabled),
				nameof(this.uPropertyPanel_Descriptor),
				nameof(this.uPropertyPanel_Type),
				nameof(this.uPropertyPanel_Stamp),
			]);
			return;
		}

		public async void uObjectList_Collapsed (
			TreeView                   sender,
			TreeViewCollapsedEventArgs args
		) {
			var senders = sender.As<TreeView>();
			GF.AssertTest(this.IsLoaded);
			this.NotifyPropertyChanged([
				nameof(this.uObjectList_SelectedItem),
			]);
			return;
		}

		public async void uObjectList_Expanding (
			TreeView                   sender,
			TreeViewExpandingEventArgs args
		) {
			var senders = sender.As<TreeView>();
			GF.AssertTest(this.IsLoaded);
			this.NotifyPropertyChanged([
				nameof(this.uObjectList_SelectedItem),
			]);
			return;
		}

		#endregion

		#region property panel

		public GameReflectionModel.DescriptorMap uPropertyPanel_Descriptor {
			get {
				if (!this.IsLoaded) {
					return new () { Enumeration = [], Object = [] };
				}
				return this.DescriptorMap;
			}
		}

		public String uPropertyPanel_Type {
			get {
				if (!this.IsLoaded) {
					return "";
				}
				return this.uObjectList_SelectedItem?.Descriptor.Name ?? "";
			}
		}

		public UniqueStamp uPropertyPanel_Stamp {
			get {
				return UniqueStamp.Create();
			}
		}

		// ----------------

		public Boolean uPropertyExport_IsEnabled {
			get {
				if (!this.IsLoaded) {
					return false;
				}
				return this.uObjectList_SelectedItem != null;
			}
		}

		public async void uPropertyExport_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			GF.AssertTest(this.IsLoaded);
			var objectType = new GameReflectionModel.CompositeType() { Primitive = GameReflectionModel.PrimitiveType.Object, Element = null, Class = this.uObjectList_SelectedItem.AsNotNull().Descriptor.Name };
			var objectValue = GameReflectionHelper.GenerateDataValueDefault(this.DescriptorMap, objectType, true).As<GameReflectionModel.FixedObject>();
			var resultValue = GameReflectionHelper.MakeDataValue(this.DescriptorMap, objectType, objectValue);
			var resultText = JsonHelper.SerializeText(resultValue);
			Clipboard.SetContent(new DataPackage().SelfAlso((it) => { it.SetText(resultText); }));
			await App.MainWindow.PushNotification(InfoBarSeverity.Success, "Copied!", "");
			return;
		}

		#endregion

	}

	public partial class ObjectItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public GameReflectionModel.ObjectDescriptor Descriptor { get; set; } = default!;

		public List<ObjectItemController> Children { get; set; } = default!;

		#endregion

		#region view

		public List<ObjectItemController> uRoot_ItemsSource {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Children;
			}
		}

		// ----------------

		public String uContent_Name {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Descriptor.Name;
			}
		}

		public Style uContentProperty_Style {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Host.View.FindResource(this.Descriptor.Property.Count switch {
					0 => "InformationalValueInfoBadgeStyle",
					_ => "AttentionValueInfoBadgeStyle",
				}).As<Style>();
			}
		}

		public Size uContentProperty_Value {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Descriptor.Property.Count;
			}
		}

		public Style uContentDerived_Style {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Host.View.FindResource(this.Descriptor.Derived.Count switch {
					0 => "InformationalValueInfoBadgeStyle",
					_ => "AttentionValueInfoBadgeStyle",
				}).As<Style>();
			}
		}

		public Size uContentDerived_Value {
			get {
				GF.AssertTest(this.Host.IsLoaded);
				return this.Descriptor.Derived.Count;
			}
		}

		#endregion

	}

}
