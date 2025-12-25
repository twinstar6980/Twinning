#pragma warning disable 0,
// ReSharper disable

using Twinning.AssistantPlus;
using Twinning.AssistantPlus.Utility;
using Windows.ApplicationModel.DataTransfer;

namespace Twinning.AssistantPlus.View.PopcapReflectionDescriptor {

	public sealed partial class ObjectPanel : CustomControl {

		#region life

		private ObjectPanelController Controller { get; }

		// ----------------

		public ObjectPanel (
		) {
			this.InitializeComponent();
			this.Controller = new () { View = this };
			return;
		}

		// ----------------

		protected override async Task StampUpdate (
		) {
			await this.Controller.UpdateView();
			return;
		}

		#endregion

		#region property

		public static readonly DependencyProperty DescriptorProperty = DependencyProperty.Register(
			nameof(ObjectPanel.Descriptor),
			typeof(PopcapReflectionModel.DescriptorMap),
			typeof(ObjectPanel),
			new (new PopcapReflectionModel.DescriptorMap() {
				Enumeration = [],
				Object = [],
			})
		);

		public PopcapReflectionModel.DescriptorMap Descriptor {
			get => this.GetValue(ObjectPanel.DescriptorProperty).As<PopcapReflectionModel.DescriptorMap>();
			set => this.SetValue(ObjectPanel.DescriptorProperty, value);
		}

		// ----------------

		public static readonly DependencyProperty TypeProperty = DependencyProperty.Register(
			nameof(ObjectPanel.Type),
			typeof(String),
			typeof(ObjectPanel),
			new ("")
		);

		public String Type {
			get => this.GetValue(ObjectPanel.TypeProperty).As<String>();
			set => this.SetValue(ObjectPanel.TypeProperty, value);
		}

		#endregion

	}

	public partial class ObjectPanelController : CustomController {

		#region data

		public ObjectPanel View { get; init; } = default!;

		// ----------------

		public PopcapReflectionModel.DescriptorMap Descriptor => this.View.Descriptor;

		public String Type => this.View.Type;

		// ----------------

		public List<PopcapReflectionModel.ObjectDescriptor>? DescriptorList { get; set; } = null;

		// ----------------

		[MemberNotNullWhen(true, nameof(ObjectPanelController.DescriptorList))]
		public Boolean IsLoaded {
			get {
				return this.DescriptorList != null;
			}
		}

		// ----------------

		public async Task UpdateView (
		) {
			if (this.Type.Length == 0) {
				this.DescriptorList = null;
				this.uGroup_ItemsSource = [];
			}
			else {
				this.DescriptorList = this.Descriptor.Object[this.Type];
				this.uGroup_ItemsSource = this.DescriptorList.Select((value, index) => (new ObjectPropertyGroupItemController() {
					Host = this,
					Index = index,
				})).ToList();
			}
			this.NotifyPropertyChanged([
				nameof(this.uGroup_ItemsSource),
			]);
			return;
		}

		#endregion

		#region group

		public List<ObjectPropertyGroupItemController> uGroup_ItemsSource { get; set; } = [];

		#endregion

	}

	public partial class ObjectPropertyGroupItemController : CustomController {

		#region data

		public ObjectPanelController Host { get; init; } = default!;

		// ----------------

		public Size Index { get; set; } = default!;

		#endregion

		#region view

		public String uTitle_ToolTip {
			get {
				AssertTest(this.Host.IsLoaded);
				var model = this.Host.DescriptorList[this.Index];
				return model.Name;
			}
		}

		public String uTitle_Text {
			get {
				AssertTest(this.Host.IsLoaded);
				var model = this.Host.DescriptorList[this.Index];
				return model.Name;
			}
		}

		// ----------------

		public List<ObjectPropertyItemController> uList_ItemsSource {
			get {
				AssertTest(this.Host.IsLoaded);
				var model = this.Host.DescriptorList[this.Index];
				return Enumerable.Range(0, model.Property.Count).Select((propertyIndex) => (new ObjectPropertyItemController() { Host = this.Host, Index = new (this.Index, propertyIndex) })).ToList();
			}
		}

		#endregion

	}

	public partial class ObjectPropertyItemController : CustomController {

		#region data

		public ObjectPanelController Host { get; init; } = default!;

		// ----------------

		public Tuple<Size, Size> Index { get; set; } = default!;

		#endregion

		#region view

		public async void uName_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			AssertTest(this.Host.IsLoaded);
			var model = this.Host.DescriptorList[this.Index.Item1].Property[this.Index.Item2];
			Clipboard.SetContent(new DataPackage().SelfAlso((it) => { it.SetText(model.Name); }));
			await App.MainWindow.PushNotification(InfoBarSeverity.Success, "Copied!", "");
			return;
		}

		public String uNameText_ToolTip {
			get {
				AssertTest(this.Host.IsLoaded);
				var model = this.Host.DescriptorList[this.Index.Item1].Property[this.Index.Item2];
				return model.Name;
			}
		}

		public String uNameText_Text {
			get {
				AssertTest(this.Host.IsLoaded);
				var model = this.Host.DescriptorList[this.Index.Item1].Property[this.Index.Item2];
				return model.Name;
			}
		}

		// ----------------

		public String uType_Text {
			get {
				AssertTest(this.Host.IsLoaded);
				var model = this.Host.DescriptorList[this.Index.Item1].Property[this.Index.Item2];
				return PopcapReflectionHelper.MakeTypeName(model.Type);
			}
		}

		// ----------------

		public String uDescription_Text {
			get {
				AssertTest(this.Host.IsLoaded);
				var model = this.Host.DescriptorList[this.Index.Item1].Property[this.Index.Item2];
				return model.Description;
			}
		}

		#endregion

	}

}
