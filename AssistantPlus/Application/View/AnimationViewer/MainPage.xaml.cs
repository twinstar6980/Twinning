#pragma warning disable 0,
// ReSharper disable

using AssistantPlus;
using AssistantPlus.Utility;
using Windows.ApplicationModel.DataTransfer;
using Windows.Globalization.NumberFormatting;
using Microsoft.UI.Xaml.Input;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Media.Imaging;
using Microsoft.UI.Xaml.Media.Animation;
using Microsoft.UI.Xaml.Navigation;
using FluentIconGlyph = AssistantPlus.Control.FluentIconGlyph;

namespace AssistantPlus.View.AnimationViewer {

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

		public Floater StageScale { get; set; } = 1.0;

		public Floater StagePositionX { get; set; } = 0.0;

		public Floater StagePositionY { get; set; } = 0.0;

		// ----------------

		public Boolean ImmediateSelect { get; set; } = default!;

		public Boolean AutomaticPlay { get; set; } = default!;

		public Boolean RepeatPlay { get; set; } = default!;

		public Boolean KeepSpeed { get; set; } = default!;

		// ----------------

		public String? AnimationFile { get; set; } = null;

		public String? TextureDirectory { get; set; } = null;

		// ----------------

		public GameAnimationModel.Animation? Animation { get; set; } = null;

		public Dictionary<String, BitmapSource>? Texture { get; set; } = null;

		public List<Boolean>? ImageFilter { get; set; } = null;

		public List<Boolean>? SpriteFilter { get; set; } = null;

		// ----------------

		public Tuple<Boolean, Size>? ActiveTarget { get; set; } = null;

		public GameAnimationModel.Sprite? ActiveSprite { get; set; } = null;

		public List<Tuple<String, GameAnimationHelper.FrameRange>>? ActiveFrameLabel { get; set; } = null;

		public GameAnimationHelper.FrameRange? ActiveFrameRange { get; set; } = null;

		public Floater? ActiveFrameSpeed { get; set; } = null;

		public Boolean? ActiveProgressState { get; set; } = null;

		// ----------------

		public Boolean ShowBoundary { get; set; } = default!;

		public String ImageFilterRule { get; set; } = default!;

		public String SpriteFilterRule { get; set; } = default!;

		public List<String>? PlantCustomLayerName { get; set; } = null;

		public List<String>? ZombieStateLayerName { get; set; } = null;

		public List<String>? ZombieGroundSwatchLayerName { get; set; } = null;

		// ----------------

		public Boolean SuppressFilterListSelectionChanged { get; set; } = false;

		public Boolean SuppressApplyFilterChanged { get; set; } = false;

		#endregion

		#region initialize

		public void Initialize (
		) {
			this.ImmediateSelect = App.Setting.Data.AnimationViewer.ImmediateSelect;
			this.AutomaticPlay = App.Setting.Data.AnimationViewer.AutomaticPlay;
			this.RepeatPlay = App.Setting.Data.AnimationViewer.RepeatPlay;
			this.KeepSpeed = App.Setting.Data.AnimationViewer.KeepSpeed;
			this.ShowBoundary = App.Setting.Data.AnimationViewer.ShowBoundary;
			this.ImageFilterRule = App.Setting.Data.AnimationViewer.ImageFilterRule;
			this.SpriteFilterRule = App.Setting.Data.AnimationViewer.SpriteFilterRule;
			this.View.uSprite.HoldEnd = true;
			this.View.uSprite.RepeatPlay = true;
			this.View.uSprite.ShowBoundary = this.ShowBoundary;
			this.View.uActiveProgress.AddHandler(UIElement.PointerPressedEvent, new PointerEventHandler(this.uActiveProgress_PointerPressed), true);
			this.View.uActiveProgress.AddHandler(UIElement.PointerReleasedEvent, new PointerEventHandler(this.uActiveProgress_PointerReleased), true);
			return;
		}

		public async Task ApplyOption (
			List<String> optionView
		) {
			await ControlHelper.WaitUntilLoaded(this.View);
			var optionImmediateSelect = default(Boolean?);
			var optionAutomaticPlay = default(Boolean?);
			var optionRepeatPlay = default(Boolean?);
			var optionKeepSpeed = default(Boolean?);
			var optionShowBoundary = default(Boolean?);
			var optionImageFilterRule = default(String?);
			var optionSpriteFilterRule = default(String?);
			var optionAnimationFile = default(String?);
			var optionTextureDirectory = default(String?);
			var optionActiveTarget = default(Tuple<Boolean, Integer>?);
			var optionActiveFrameRange = default(Tuple<Integer, Integer>?);
			var optionActiveFrameSpeed = default(Floater?);
			var optionActiveProgressState = default(Boolean?);
			try {
				var option = new CommandLineReader(optionView);
				if (option.Check("-ImmediateSelect")) {
					optionImmediateSelect = option.NextBoolean();
				}
				if (option.Check("-AutomaticPlay")) {
					optionAutomaticPlay = option.NextBoolean();
				}
				if (option.Check("-RepeatPlay")) {
					optionRepeatPlay = option.NextBoolean();
				}
				if (option.Check("-KeepSpeed")) {
					optionKeepSpeed = option.NextBoolean();
				}
				if (option.Check("-ShowBoundary")) {
					optionShowBoundary = option.NextBoolean();
				}
				if (option.Check("-ImageFilterRule")) {
					optionImageFilterRule = option.NextString();
				}
				if (option.Check("-SpriteFilterRule")) {
					optionSpriteFilterRule = option.NextString();
				}
				if (option.Check("-AnimationFile")) {
					optionAnimationFile = option.NextString();
				}
				if (option.Check("-TextureDirectory")) {
					optionTextureDirectory = option.NextString();
				}
				if (option.Check("-ActiveTarget")) {
					optionActiveTarget = new (
						option.NextBoolean(),
						option.NextInteger()
					);
				}
				if (option.Check("-ActiveFrameRange")) {
					optionActiveFrameRange = new (
						option.NextInteger(),
						option.NextInteger()
					);
				}
				if (option.Check("-ActiveFrameSpeed")) {
					optionActiveFrameSpeed = option.NextFloater();
				}
				if (option.Check("-ActiveProgressState")) {
					optionActiveProgressState = option.NextBoolean();
				}
				if (!option.Done()) {
					throw new ($"Too many option : '{String.Join(' ', option.NextStringList())}'.");
				}
			}
			catch (Exception e) {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			if (optionImmediateSelect != null) {
				this.ImmediateSelect = optionImmediateSelect.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uImmediateSelect_IsChecked)
				);
			}
			if (optionAutomaticPlay != null) {
				this.AutomaticPlay = optionAutomaticPlay.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uAutomaticPlay_IsChecked)
				);
			}
			if (optionRepeatPlay != null) {
				this.RepeatPlay = optionRepeatPlay.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uRepeatPlay_IsChecked)
				);
			}
			if (optionKeepSpeed != null) {
				this.KeepSpeed = optionKeepSpeed.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uKeepSpeed_IsChecked)
				);
			}
			if (optionShowBoundary != null) {
				this.ShowBoundary = optionShowBoundary.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uShowBoundary_IsChecked)
				);
			}
			if (optionImageFilterRule != null) {
				this.ImageFilterRule = optionImageFilterRule;
				this.NotifyPropertyChanged(
					nameof(this.uImageFilterRuleText_Text)
				);
			}
			if (optionSpriteFilterRule != null) {
				this.SpriteFilterRule = optionSpriteFilterRule;
				this.NotifyPropertyChanged(
					nameof(this.uSpriteFilterRuleText_Text)
				);
			}
			if (optionAnimationFile != null) {
				await this.ApplyLoad(
					optionAnimationFile,
					optionTextureDirectory,
					optionActiveTarget == null ? null : new (optionActiveTarget.Item1, (Size)optionActiveTarget.Item2),
					optionActiveFrameRange == null ? null : new () { Start = (Size)optionActiveFrameRange.Item1, Duration = (Size)optionActiveFrameRange.Item2 },
					optionActiveFrameSpeed,
					optionActiveProgressState
				);
			}
			return;
		}

		public async Task<List<String>> CollectOption (
		) {
			var option = new CommandLineWriter();
			if (option.Check("-ImmediateSelect")) {
				option.NextBoolean(this.ImmediateSelect);
			}
			if (option.Check("-AutomaticPlay")) {
				option.NextBoolean(this.AutomaticPlay);
			}
			if (option.Check("-RepeatPlay")) {
				option.NextBoolean(this.RepeatPlay);
			}
			if (option.Check("-KeepSpeed")) {
				option.NextBoolean(this.KeepSpeed);
			}
			if (option.Check("-ShowBoundary")) {
				option.NextBoolean(this.ShowBoundary);
			}
			if (option.Check("-ImageFilterRule")) {
				option.NextString(this.ImageFilterRule);
			}
			if (option.Check("-SpriteFilterRule")) {
				option.NextString(this.SpriteFilterRule);
			}
			if (option.Check("-AnimationFile", this.Loaded)) {
				option.NextString(this.AnimationFile.AsNotNull());
			}
			if (option.Check("-TextureDirectory", this.Loaded)) {
				option.NextString(this.TextureDirectory.AsNotNull());
			}
			if (option.Check("-ActiveTarget", this.Activated)) {
				option.NextBoolean(this.ActiveTarget.AsNotNull().Item1);
				option.NextInteger(this.ActiveTarget.AsNotNull().Item2);
			}
			if (option.Check("-ActiveFrameRange", this.Activated)) {
				option.NextInteger(this.ActiveFrameRange.AsNotNull().Start);
				option.NextInteger(this.ActiveFrameRange.AsNotNull().Duration);
			}
			if (option.Check("-ActiveFrameSpeed", this.Activated)) {
				option.NextFloater(this.ActiveFrameSpeed.AsNotNull());
			}
			if (option.Check("-ActiveProgressState", this.Activated)) {
				option.NextBoolean(this.ActiveProgressState.AsNotNull());
			}
			return option.Done();
		}

		public async Task<Boolean> RequestClose (
		) {
			return true;
		}

		#endregion

		#region load & activate

		[MemberNotNullWhen(true, nameof(MainPageController.AnimationFile))]
		[MemberNotNullWhen(true, nameof(MainPageController.TextureDirectory))]
		[MemberNotNullWhen(true, nameof(MainPageController.Animation))]
		[MemberNotNullWhen(true, nameof(MainPageController.Texture))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.SpriteFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.PlantCustomLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieStateLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieGroundSwatchLayerName))]
		public Boolean Loaded {
			get {
				return this.Animation != null;
			}
		}

		[MemberNotNullWhen(true, nameof(MainPageController.AnimationFile))]
		[MemberNotNullWhen(true, nameof(MainPageController.TextureDirectory))]
		[MemberNotNullWhen(true, nameof(MainPageController.Animation))]
		[MemberNotNullWhen(true, nameof(MainPageController.Texture))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.SpriteFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.PlantCustomLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieStateLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieGroundSwatchLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ActiveTarget))]
		[MemberNotNullWhen(true, nameof(MainPageController.ActiveSprite))]
		[MemberNotNullWhen(true, nameof(MainPageController.ActiveFrameLabel))]
		[MemberNotNullWhen(true, nameof(MainPageController.ActiveFrameRange))]
		[MemberNotNullWhen(true, nameof(MainPageController.ActiveFrameSpeed))]
		[MemberNotNullWhen(true, nameof(MainPageController.ActiveProgressState))]
		public Boolean Activated {
			get {
				return this.Loaded && this.ActiveTarget != null;
			}
		}

		// ----------------

		public async Task Load (
			String animationFile,
			String textureDirectory
		) {
			GF.AssertTest(!this.Loaded && !this.Activated);
			var animation = default(GameAnimationModel.Animation);
			var texture = default(Dictionary<String, BitmapSource>);
			try {
				animation = await GameAnimationHelper.LoadAnimation(animationFile);
				texture = await GameAnimationHelper.LoadTexture(textureDirectory, animation);
			}
			catch (Exception e) {
				App.MainWindow.PushNotification(InfoBarSeverity.Error, "Failed to load animation.", e.ToString());
				return;
			}
			this.AnimationFile = animationFile;
			this.TextureDirectory = textureDirectory;
			this.Animation = animation;
			this.Texture = texture;
			this.ImageFilter = Enumerable.Repeat(false, this.Animation.Image.Count).ToList();
			this.SpriteFilter = Enumerable.Repeat(false, this.Animation.Sprite.Count).ToList();
			this.PlantCustomLayerName = this.Animation.Sprite.Where((value) => (value.Name != null)).Select((value) => (value.Name.AsNotNull())).Where((value) => (value.StartsWith("custom_"))).ToList();
			this.ZombieStateLayerName = this.Animation.Sprite.Where((value) => (value.Name != null)).Select((value) => (value.Name.AsNotNull())).Where((value) => (value == "ink" || value == "butter")).ToList();
			this.ZombieGroundSwatchLayerName = this.Animation.Sprite.Where((value) => (value.Name != null)).Select((value) => (value.Name.AsNotNull())).Where((value) => (value == "ground_swatch" || value == "ground_swatch_plane")).ToList();
			this.NotifyPropertyChanged(
				nameof(this.uClearSource_IsEnabled),
				nameof(this.uAnimationFile_IsEnabled),
				nameof(this.uAnimationFile_Text),
				nameof(this.uTextureDirectory_IsEnabled),
				nameof(this.uTextureDirectory_Text),
				nameof(this.uTextureDirectoryPick_IsEnabled),
				nameof(this.uImageList_ItemsSource),
				nameof(this.uSpriteList_ItemsSource),
				nameof(this.uMainSpriteList_ItemsSource)
			);
			this.View.uImageList.SelectAll();
			this.View.uSpriteList.SelectAll();
			if (this.Animation.MainSprite != null) {
				this.View.uMainSpriteList.SelectAll();
			}
			this.NotifyPropertyChanged(
				nameof(this.uPlantCustomLayerIcon_Opacity),
				nameof(this.uPlantCustomLayer_IsEnabled),
				nameof(this.uPlantCustomLayer_ItemsSource),
				nameof(this.uPlantCustomLayer_SelectedItem),
				nameof(this.uZombieStateLayerIcon_Opacity),
				nameof(this.uZombieStateLayer_IsEnabled),
				nameof(this.uZombieStateLayer_ItemsSource),
				nameof(this.uZombieStateLayer_SelectedItem),
				nameof(this.uZombieGroundSwatchLayerIcon_Opacity),
				nameof(this.uZombieGroundSwatchLayer_IsEnabled),
				nameof(this.uZombieGroundSwatchLayer_IsChecked)
			);
			return;
		}

		public async Task Unload (
		) {
			GF.AssertTest(this.Loaded && !this.Activated);
			this.View.uImageList.DeselectRange(new (0, (USize)this.Animation.Image.Count));
			this.View.uSpriteList.DeselectRange(new (0, (USize)this.Animation.Sprite.Count));
			if (this.Animation.MainSprite != null) {
				this.View.uMainSpriteList.DeselectRange(new (0, 1));
			}
			this.AnimationFile = null;
			this.TextureDirectory = null;
			this.Animation = null;
			this.Texture = null;
			this.ImageFilter = null;
			this.SpriteFilter = null;
			this.PlantCustomLayerName = null;
			this.ZombieStateLayerName = null;
			this.ZombieGroundSwatchLayerName = null;
			this.NotifyPropertyChanged(
				nameof(this.uClearSource_IsEnabled),
				nameof(this.uAnimationFile_IsEnabled),
				nameof(this.uAnimationFile_Text),
				nameof(this.uTextureDirectory_IsEnabled),
				nameof(this.uTextureDirectory_Text),
				nameof(this.uTextureDirectoryPick_IsEnabled),
				nameof(this.uImageList_ItemsSource),
				nameof(this.uSpriteList_ItemsSource),
				nameof(this.uMainSpriteList_ItemsSource),
				nameof(this.uPlantCustomLayerIcon_Opacity),
				nameof(this.uPlantCustomLayer_IsEnabled),
				nameof(this.uPlantCustomLayer_ItemsSource),
				nameof(this.uPlantCustomLayer_SelectedItem),
				nameof(this.uZombieStateLayerIcon_Opacity),
				nameof(this.uZombieStateLayer_IsEnabled),
				nameof(this.uZombieStateLayer_ItemsSource),
				nameof(this.uZombieStateLayer_SelectedItem),
				nameof(this.uZombieGroundSwatchLayerIcon_Opacity),
				nameof(this.uZombieGroundSwatchLayer_IsEnabled),
				nameof(this.uZombieGroundSwatchLayer_IsChecked)
			);
			return;
		}

		// ----------------

		public async Task Activate (
			Tuple<Boolean, Size>            target,
			GameAnimationHelper.FrameRange? frameRange,
			Floater?                        frameSpeed,
			Boolean?                        progressState,
			TimeSpan?                       initialTime
		) {
			GF.AssertTest(this.Loaded && !this.Activated);
			var activeSprite = default(GameAnimationModel.Sprite);
			if (!target.Item1) {
				var originalTarget = GameAnimationHelper.SelectImage(this.Animation, (Size)target.Item2);
				activeSprite = new () {
					Name = GameAnimationHelper.ParseImageFileName(originalTarget.Name),
					FrameRate = null,
					WorkArea = new (0, 0),
					Frame = [
						new GameAnimationModel.Frame() {
							Label = null,
							Stop = false,
							Command = [],
							Remove = [],
							Append = [
								new GameAnimationModel.LayerAppend() {
									Index = 0,
									Name = null,
									Resource = target.Item2,
									Sprite = false,
									Additive = false,
									PreloadFrame = 0,
									TimeScale = 1.0,
								},
							],
							Change = [
								new GameAnimationModel.LayerChange() {
									Index = 0,
									Transform = [0.0, 0.0],
									Color = null,
									SpriteFrameNumber = null,
									SourceRectangle = null,
								},
							],
						},
					],
				};
			}
			else {
				var originalTarget = GameAnimationHelper.SelectSprite(this.Animation, target.Item2);
				activeSprite = originalTarget;
			}
			this.ActiveTarget = target;
			this.ActiveSprite = activeSprite;
			this.ActiveFrameLabel = GameAnimationHelper.ParseSpriteFrameLabel(activeSprite);
			this.ActiveFrameRange = frameRange ?? new () { Start = 0, Duration = activeSprite.Frame.Count };
			this.ActiveFrameSpeed = frameSpeed ?? activeSprite.FrameRate ?? (Floater)this.Animation.FrameRate;
			this.ActiveProgressState = progressState ?? this.AutomaticPlay;
			this.View.uSprite.Load(this.Animation, this.Texture, this.ImageFilter, this.SpriteFilter, activeSprite);
			GF.AssertTest(this.View.uSprite.Loaded);
			var sliderAnimation = new ObjectAnimationUsingKeyFrames();
			for (var frameIndex = 0; frameIndex < activeSprite.Frame.Count; frameIndex++) {
				sliderAnimation.KeyFrames.Add(
					new DiscreteObjectKeyFrame() {
						KeyTime = KeyTime.FromTimeSpan(TimeSpan.FromSeconds(frameIndex)),
						Value = (Floater)(frameIndex + 1),
					}
				);
			}
			Storyboard.SetTargetProperty(sliderAnimation, "Value");
			var sliderStoryboard = new Storyboard() {
				Duration = new (TimeSpan.FromSeconds(activeSprite.Frame.Count)),
				RepeatBehavior = RepeatBehavior.Forever,
				Children = {
					sliderAnimation,
				},
			};
			Storyboard.SetTarget(sliderStoryboard, this.View.uActiveProgress);
			this.View.uSprite.Storyboard.Children.Add(sliderStoryboard);
			this.View.uSprite.Storyboard.Completed += (sender, o) => {
				if (!this.RepeatPlay) {
					this.View.uSprite.State = SpriteControl.StateType.Paused;
					this.ActiveProgressState = false;
					this.NotifyPropertyChanged(
						nameof(this.uActiveProgressStateIcon_Glyph)
					);
				}
			};
			this.View.uSprite.FrameRange = this.ActiveFrameRange;
			this.View.uSprite.FrameSpeed = this.ActiveFrameSpeed.AsNotNull();
			this.View.uSprite.State = !this.ActiveProgressState.AsNotNull() ? SpriteControl.StateType.Paused : SpriteControl.StateType.Playing;
			this.View.uSprite.CurrentTime = initialTime ?? TimeSpan.FromSeconds(this.ActiveFrameRange.Start);
			this.NotifyPropertyChanged(
				nameof(this.uActiveFrameRangeIcon_Opacity),
				nameof(this.uActiveFrameRangeBegin_IsEnabled),
				nameof(this.uActiveFrameRangeBegin_Minimum),
				nameof(this.uActiveFrameRangeBegin_Maximum),
				nameof(this.uActiveFrameRangeBegin_Value),
				nameof(this.uActiveFrameRangeEnd_IsEnabled),
				nameof(this.uActiveFrameRangeEnd_Minimum),
				nameof(this.uActiveFrameRangeEnd_Maximum),
				nameof(this.uActiveFrameRangeEnd_Value),
				nameof(this.uActiveFrameRangeLabelIcon_Opacity),
				nameof(this.uActiveFrameRangeLabel_IsEnabled),
				nameof(this.uActiveFrameRangeLabel_ItemsSource),
				nameof(this.uActiveFrameRangeLabel_SelectedItem),
				nameof(this.uActiveFrameSpeedIcon_Opacity),
				nameof(this.uActiveFrameSpeed_IsEnabled),
				nameof(this.uActiveFrameSpeed_Value),
				nameof(this.uActiveProgress_IsEnabled),
				nameof(this.uActiveProgress_Minimum),
				nameof(this.uActiveProgress_Maximum),
				nameof(this.uActiveProgressState_IsEnabled),
				nameof(this.uActiveProgressStateIcon_Glyph),
				nameof(this.uActiveProgressPrevious_IsEnabled),
				nameof(this.uActiveProgressNext_IsEnabled)
			);
			foreach (var item in this.View.uImageList.ItemsSource.As<List<MainPageImageItemController>>()) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			foreach (var item in this.View.uSpriteList.ItemsSource.As<List<MainPageSpriteItemController>>()) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			foreach (var item in this.View.uMainSpriteList.ItemsSource.As<List<MainPageMainSpriteItemController>>()) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			return;
		}

		public async Task Deactivate (
		) {
			GF.AssertTest(this.Loaded && this.Activated);
			this.ActiveTarget = null;
			this.ActiveSprite = null;
			this.ActiveFrameLabel = null;
			this.ActiveFrameRange = null;
			this.ActiveFrameSpeed = null;
			this.ActiveProgressState = null;
			this.NotifyPropertyChanged(
				nameof(this.uActiveFrameRangeIcon_Opacity),
				nameof(this.uActiveFrameRangeBegin_IsEnabled),
				nameof(this.uActiveFrameRangeBegin_Minimum),
				nameof(this.uActiveFrameRangeBegin_Maximum),
				nameof(this.uActiveFrameRangeBegin_Value),
				nameof(this.uActiveFrameRangeEnd_IsEnabled),
				nameof(this.uActiveFrameRangeEnd_Minimum),
				nameof(this.uActiveFrameRangeEnd_Maximum),
				nameof(this.uActiveFrameRangeEnd_Value),
				nameof(this.uActiveFrameRangeLabelIcon_Opacity),
				nameof(this.uActiveFrameRangeLabel_IsEnabled),
				nameof(this.uActiveFrameRangeLabel_ItemsSource),
				nameof(this.uActiveFrameRangeLabel_SelectedItem),
				nameof(this.uActiveFrameSpeedIcon_Opacity),
				nameof(this.uActiveFrameSpeed_IsEnabled),
				nameof(this.uActiveFrameSpeed_Value),
				nameof(this.uActiveProgress_IsEnabled),
				nameof(this.uActiveProgress_Minimum),
				nameof(this.uActiveProgress_Maximum),
				nameof(this.uActiveProgressState_IsEnabled),
				nameof(this.uActiveProgressStateIcon_Glyph),
				nameof(this.uActiveProgressPrevious_IsEnabled),
				nameof(this.uActiveProgressNext_IsEnabled)
			);
			foreach (var item in this.View.uImageList.ItemsSource.As<List<MainPageImageItemController>>()) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			foreach (var item in this.View.uSpriteList.ItemsSource.As<List<MainPageSpriteItemController>>()) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			foreach (var item in this.View.uMainSpriteList.ItemsSource.As<List<MainPageMainSpriteItemController>>()) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			this.View.uSprite.Unload();
			return;
		}

		public async Task UpdateActiveFrameRange (
			GameAnimationHelper.FrameRange frameRange
		) {
			GF.AssertTest(this.Loaded && this.Activated);
			this.ActiveFrameRange = frameRange;
			this.View.uSprite.FrameRange = frameRange;
			this.View.uSprite.CurrentTime = TimeSpan.FromSeconds(frameRange.Start);
			this.NotifyPropertyChanged(
				nameof(this.uActiveFrameRangeBegin_Value),
				nameof(this.uActiveFrameRangeEnd_Value),
				nameof(this.uActiveFrameRangeLabel_SelectedItem),
				nameof(this.uActiveProgress_Minimum),
				nameof(this.uActiveProgress_Maximum),
				nameof(this.uActiveProgressStateIcon_Glyph)
			);
			return;
		}

		// ----------------

		public async Task ApplyLoad (
			String                          animationFile,
			String?                         textureDirectory,
			Tuple<Boolean, Size>?           target,
			GameAnimationHelper.FrameRange? frameRange,
			Floater?                        frameSpeed,
			Boolean?                        progressState
		) {
			textureDirectory ??= StorageHelper.Parent(animationFile).AsNotNull();
			if (this.Loaded) {
				if (this.Activated) {
					await this.Deactivate();
				}
				await this.Unload();
			}
			await this.Load(animationFile, textureDirectory);
			if (this.Loaded) {
				await this.ApplyFilterRule();
				if (this.ImmediateSelect && this.Animation.MainSprite != null) {
					await this.Activate(target ?? new (true, this.Animation.Sprite.Count), frameRange, frameSpeed, progressState, null);
				}
				await App.Instance.AppendRecentLauncherItem(new () {
					Title = Regex.Replace(StorageHelper.Name(animationFile), @"(\.pam\.json)$", "", RegexOptions.IgnoreCase),
					Type = ModuleType.AnimationViewer,
					Option = await this.CollectOption(),
					Command = [],
				});
			}
			return;
		}

		public async Task ApplyFilter (
			List<Boolean?>? imageFilter,
			List<Boolean?>? spriteFilter
		) {
			GF.AssertTest(this.Loaded);
			if (this.SuppressApplyFilterChanged) {
				return;
			}
			this.SuppressApplyFilterChanged = true;
			var target = this.ActiveTarget;
			var frameRange = this.ActiveFrameRange;
			var frameSpeed = this.ActiveFrameSpeed;
			var currentState = this.ActiveProgressState;
			var currentTime = !this.Activated ? (TimeSpan?)null : this.View.uSprite.CurrentTime;
			if (target != null) {
				await this.Deactivate();
			}
			this.SuppressFilterListSelectionChanged = true;
			if (imageFilter != null) {
				for (var index = 0; index < imageFilter.Count; index++) {
					var state = imageFilter[index];
					if (state == null || state.AsNotNull() == this.ImageFilter[index]) {
						continue;
					}
					this.ImageFilter[index] = state.AsNotNull();
					if (state.AsNotNull()) {
						this.View.uImageList.SelectRange(new (index, 1));
					}
					else {
						this.View.uImageList.DeselectRange(new (index, 1));
					}
				}
			}
			if (spriteFilter != null) {
				for (var index = 0; index < spriteFilter.Count; index++) {
					var state = spriteFilter[index];
					if (state == null || state.AsNotNull() == this.SpriteFilter[index]) {
						continue;
					}
					this.SpriteFilter[index] = state.AsNotNull();
					if (state.AsNotNull()) {
						this.View.uSpriteList.SelectRange(new (index, 1));
					}
					else {
						this.View.uSpriteList.DeselectRange(new (index, 1));
					}
				}
			}
			this.SuppressFilterListSelectionChanged = false;
			this.NotifyPropertyChanged(
				nameof(this.uPlantCustomLayer_SelectedItem),
				nameof(this.uZombieStateLayer_SelectedItem),
				nameof(this.uZombieGroundSwatchLayer_IsChecked)
			);
			if (target != null) {
				await this.Activate(target, frameRange, frameSpeed, currentState, currentTime);
			}
			this.SuppressApplyFilterChanged = false;
			return;
		}

		public async Task ApplyFilterRule (
		) {
			GF.AssertTest(this.Loaded);
			await this.ApplyFilter(
				this.Animation.Image.Select((value) => (this.ImageFilterRule.Length != 0 && Regex.IsMatch(GameAnimationHelper.ParseImageFileName(value.Name), this.ImageFilterRule) ? false : (Boolean?)null)).ToList(),
				this.Animation.Sprite.Select((value) => (value.Name != null && this.SpriteFilterRule.Length != 0 && Regex.IsMatch(value.Name, this.SpriteFilterRule) ? false : (Boolean?)null)).ToList()
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
				var animationFile = StorageHelper.GetLongPath(item[0].Path);
				if (!StorageHelper.ExistFile(animationFile)) {
					App.MainWindow.PushNotification(InfoBarSeverity.Error, "Source is not a file.", "");
					return;
				}
				await this.ApplyLoad(animationFile, null, null, null, null, null);
			}
			return;
		}

		#endregion

		#region stage

		public String uStagePositionX_Text {
			get {
				return $"{this.StagePositionX:F0}";
			}
		}

		public String uStagePositionY_Text {
			get {
				return $"{this.StagePositionY:F0}";
			}
		}

		public String uStageScale_Text {
			get {
				return $"{this.StageScale * 100:F0} %";
			}
		}

		public String uStageScaleIcon_Glyph {
			get {
				var valueLose = (Size)Math.Round(this.StageScale * 100.0);
				return valueLose switch {
					< 100 => FluentIconGlyph.BackToWindow,
					100   => FluentIconGlyph.Reshare,
					> 100 => FluentIconGlyph.FullScreen,
				};
			}
		}

		// ----------------

		public async void uStage_ViewChanged (
			Object sender,
			Object args
		) {
			var senders = sender.As<ScrollViewer>();
			this.StageScale = senders.ZoomFactor;
			this.StagePositionX = (senders.HorizontalOffset + senders.ActualWidth / 2) / senders.ZoomFactor;
			this.StagePositionY = (senders.VerticalOffset + senders.ActualHeight / 2) / senders.ZoomFactor;
			this.NotifyPropertyChanged(
				nameof(this.uStagePositionX_Text),
				nameof(this.uStagePositionY_Text),
				nameof(this.uStageScale_Text),
				nameof(this.uStageScaleIcon_Glyph)
			);
			return;
		}

		#endregion

		#region setting

		public Boolean uImmediateSelect_IsChecked {
			get {
				return this.ImmediateSelect;
			}
		}

		public async void uImmediateSelect_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.ImmediateSelect = senders.IsChecked.AsNotNull();
			return;
		}

		// ----------------

		public Boolean uAutomaticPlay_IsChecked {
			get {
				return this.AutomaticPlay;
			}
		}

		public async void uAutomaticPlay_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.AutomaticPlay = senders.IsChecked.AsNotNull();
			return;
		}

		// ----------------

		public Boolean uRepeatPlay_IsChecked {
			get {
				return this.RepeatPlay;
			}
		}

		public async void uRepeatPlay_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.RepeatPlay = senders.IsChecked.AsNotNull();
			this.View.uSprite.RepeatPlay = this.RepeatPlay;
			return;
		}

		// ----------------

		public Boolean uKeepSpeed_IsChecked {
			get {
				return this.KeepSpeed;
			}
		}

		public async void uKeepSpeed_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.KeepSpeed = senders.IsChecked.AsNotNull();
			return;
		}

		#endregion

		#region source

		public Boolean uClearSource_IsEnabled {
			get {
				if (!this.Loaded) {
					return false;
				}
				return true;
			}
		}

		public async void uClearSource_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (!this.Loaded) {
				return;
			}
			if (this.Activated) {
				await this.Deactivate();
			}
			await this.Unload();
			return;
		}

		// ----------------

		public Boolean uAnimationFile_IsEnabled {
			get {
				if (!this.Loaded) {
					return false;
				}
				return true;
			}
		}

		public String uAnimationFile_Text {
			get {
				if (!this.Loaded) {
					return "";
				}
				return this.AnimationFile;
			}
		}

		public async void uAnimationFilePick_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			var isPlaying = this.Activated && this.View.uSprite.State == SpriteControl.StateType.Playing;
			if (isPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
			}
			var animationFile = await StorageHelper.PickLoadFile(WindowHelper.Find(this.View), $"{nameof(AnimationViewer)}.AnimationFile");
			if (animationFile != null) {
				await this.ApplyLoad(animationFile, null, null, null, null, null);
			}
			else {
				if (isPlaying) {
					this.View.uSprite.State = SpriteControl.StateType.Playing;
				}
			}
			return;
		}

		// ----------------

		public Boolean uTextureDirectory_IsEnabled {
			get {
				if (!this.Loaded) {
					return false;
				}
				return true;
			}
		}

		public String uTextureDirectory_Text {
			get {
				if (!this.Loaded) {
					return "";
				}
				return this.TextureDirectory;
			}
		}

		public Boolean uTextureDirectoryPick_IsEnabled {
			get {
				if (!this.Loaded) {
					return false;
				}
				return true;
			}
		}

		public async void uTextureDirectoryPick_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (!this.Loaded) {
				return;
			}
			var isPlaying = this.Activated && this.View.uSprite.State == SpriteControl.StateType.Playing;
			if (isPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
			}
			var textureDirectory = await StorageHelper.PickLoadDirectory(WindowHelper.Find(this.View), $"{nameof(AnimationViewer)}.TextureDirectory");
			if (textureDirectory != null) {
				await this.ApplyLoad(this.AnimationFile, textureDirectory, null, null, null, null);
			}
			else {
				if (isPlaying) {
					this.View.uSprite.State = SpriteControl.StateType.Playing;
				}
			}
			return;
		}

		#endregion

		#region image & sprite

		public List<MainPageImageItemController> uImageList_ItemsSource {
			get {
				if (!this.Loaded) {
					return [];
				}
				return this.Animation.Image.Select((value, index) => (new MainPageImageItemController() { Host = this, Index = index })).ToList();
			}
		}

		public async void uImageList_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ListView>();
			if (!this.Loaded) {
				return;
			}
			if (this.SuppressFilterListSelectionChanged) { return; }
			foreach (var item in args.AddedItems.Select(GF.As<MainPageImageItemController>)) {
				this.ImageFilter[item.Index] = true;
			}
			foreach (var item in args.RemovedItems.Select(GF.As<MainPageImageItemController>)) {
				this.ImageFilter[item.Index] = false;
			}
			this.SuppressApplyFilterChanged = true;
			this.NotifyPropertyChanged(
				nameof(this.uPlantCustomLayer_SelectedItem),
				nameof(this.uZombieStateLayer_SelectedItem),
				nameof(this.uZombieGroundSwatchLayer_IsChecked)
			);
			this.SuppressApplyFilterChanged = false;
			if (this.Activated && (args.AddedItems.Count != 0 || args.RemovedItems.Count != 0)) {
				var target = this.ActiveTarget;
				var frameRange = this.ActiveFrameRange;
				var frameSpeed = this.ActiveFrameSpeed;
				var currentState = this.ActiveProgressState;
				var currentTime = this.View.uSprite.CurrentTime;
				await this.Deactivate();
				await this.Activate(target, frameRange, frameSpeed, currentState, currentTime);
			}
			return;
		}

		// ----------------

		public List<MainPageSpriteItemController> uSpriteList_ItemsSource {
			get {
				if (!this.Loaded) {
					return [];
				}
				return this.Animation.Sprite.Select((value, index) => (new MainPageSpriteItemController() { Host = this, Index = index })).ToList();
			}
		}

		public async void uSpriteList_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ListView>();
			if (!this.Loaded) {
				return;
			}
			if (this.SuppressFilterListSelectionChanged) { return; }
			foreach (var item in args.AddedItems.Select(GF.As<MainPageSpriteItemController>)) {
				this.SpriteFilter[item.Index] = true;
			}
			foreach (var item in args.RemovedItems.Select(GF.As<MainPageSpriteItemController>)) {
				this.SpriteFilter[item.Index] = false;
			}
			this.SuppressApplyFilterChanged = true;
			this.NotifyPropertyChanged(
				nameof(this.uPlantCustomLayer_SelectedItem),
				nameof(this.uZombieStateLayer_SelectedItem),
				nameof(this.uZombieGroundSwatchLayer_IsChecked)
			);
			this.SuppressApplyFilterChanged = false;
			if (this.Activated && (args.AddedItems.Count != 0 || args.RemovedItems.Count != 0)) {
				var target = this.ActiveTarget;
				var frameRange = this.ActiveFrameRange;
				var frameSpeed = this.ActiveFrameSpeed;
				var currentState = this.ActiveProgressState;
				var currentTime = this.View.uSprite.CurrentTime;
				await this.Deactivate();
				await this.Activate(target, frameRange, frameSpeed, currentState, currentTime);
			}
			return;
		}

		// ----------------

		public List<MainPageMainSpriteItemController> uMainSpriteList_ItemsSource {
			get {
				if (!this.Loaded) {
					return [new () { Host = this, Index = null }];
				}
				return [new () { Host = this, Index = this.Animation.MainSprite == null ? null : this.Animation.Sprite.Count }];
			}
		}

		public async void uMainSpriteList_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ListView>();
			if (!this.Loaded) {
				return;
			}
			if (senders.SelectedItems.Count == 0) {
				senders.SelectAll();
			}
			return;
		}

		#endregion

		#region active frame range

		public Floater uActiveFrameRangeIcon_Opacity {
			get {
				if (!this.Activated) {
					return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(false);
				}
				return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public Boolean uActiveFrameRangeBegin_IsEnabled {
			get {
				if (!this.Activated) {
					return false;
				}
				return true;
			}
		}

		public DecimalFormatter uActiveFrameRangeBegin_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uActiveFrameRangeBegin_Minimum {
			get {
				if (!this.Activated) {
					return Floater.NaN;
				}
				return 1.0;
			}
		}

		public Floater uActiveFrameRangeBegin_Maximum {
			get {
				if (!this.Activated) {
					return Floater.NaN;
				}
				return this.ActiveSprite.AsNotNull().Frame.Count;
			}
		}

		public Floater uActiveFrameRangeBegin_Value {
			get {
				if (!this.Activated) {
					return Floater.NaN;
				}
				return this.ActiveFrameRange.Start + 1;
			}
		}

		public async void uActiveFrameRangeBegin_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (!this.Activated) {
				return;
			}
			if (Floater.IsFinite(args.NewValue)) {
				var newBegin = (Size)args.NewValue - 1;
				var newRange = new GameAnimationHelper.FrameRange() {
					Start = newBegin,
					Duration = this.ActiveFrameRange.Start + this.ActiveFrameRange.Duration - newBegin,
				};
				if (newRange.Start >= this.ActiveFrameRange.Start + this.ActiveFrameRange.Duration) {
					newRange.Duration = 1;
				}
				if (newRange != this.ActiveFrameRange) {
					await this.UpdateActiveFrameRange(newRange);
				}
			}
			this.NotifyPropertyChanged(
				nameof(this.uActiveFrameRangeBegin_Value)
			);
			return;
		}

		// ----------------

		public Boolean uActiveFrameRangeEnd_IsEnabled {
			get {
				if (!this.Activated) {
					return false;
				}
				return true;
			}
		}

		public DecimalFormatter uActiveFrameRangeEnd_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uActiveFrameRangeEnd_Minimum {
			get {
				if (!this.Activated) {
					return Floater.NaN;
				}
				return 1.0;
			}
		}

		public Floater uActiveFrameRangeEnd_Maximum {
			get {
				if (!this.Activated) {
					return Floater.NaN;
				}
				return this.ActiveSprite.AsNotNull().Frame.Count;
			}
		}

		public Floater uActiveFrameRangeEnd_Value {
			get {
				if (!this.Activated) {
					return Floater.NaN;
				}
				return this.ActiveFrameRange.Start + this.ActiveFrameRange.Duration;
			}
		}

		public async void uActiveFrameRangeEnd_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (!this.Activated) {
				return;
			}
			if (Floater.IsFinite(args.NewValue)) {
				var newEnd = (Size)args.NewValue - 1;
				var newRange = new GameAnimationHelper.FrameRange() {
					Start = this.ActiveFrameRange.Start,
					Duration = newEnd - this.ActiveFrameRange.Start + 1,
				};
				if (newRange.Duration < 1) {
					newRange.Start = newEnd;
					newRange.Duration = 1;
				}
				if (newRange != this.ActiveFrameRange) {
					await this.UpdateActiveFrameRange(newRange);
				}
			}
			this.NotifyPropertyChanged(
				nameof(this.uActiveFrameRangeEnd_Value)
			);
			return;
		}

		#endregion

		#region active frame range label

		public Floater uActiveFrameRangeLabelIcon_Opacity {
			get {
				if (!this.Activated) {
					return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(false);
				}
				return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public String uActiveFrameRangeLabel__ItemNameOfAll { get; } = "\0";

		public Boolean uActiveFrameRangeLabel_IsEnabled {
			get {
				if (!this.Activated) {
					return false;
				}
				return true;
			}
		}

		public List<String> uActiveFrameRangeLabel_ItemsSource {
			get {
				if (!this.Activated) {
					return [];
				}
				return [..this.ActiveFrameLabel.Select((value) => (value.Item1)), this.uActiveFrameRangeLabel__ItemNameOfAll];
			}
		}

		public String? uActiveFrameRangeLabel_SelectedItem {
			get {
				if (!this.Activated) {
					return null;
				}
				var result = this.ActiveFrameLabel.Find((value) => (value.Item2 == this.ActiveFrameRange))?.Item1;
				return result ?? (this.ActiveFrameRange.Start == 0 && this.ActiveFrameRange.Duration == this.ActiveSprite.AsNotNull().Frame.Count ? this.uActiveFrameRangeLabel__ItemNameOfAll : null);
			}
		}

		public async void uActiveFrameRangeLabel_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (!this.Activated) {
				return;
			}
			if (args.AddedItems.Count == 1) {
				var newLabel = args.AddedItems[0].As<String>();
				var newRange = new GameAnimationHelper.FrameRange() {
					Start = 0,
					Duration = this.ActiveSprite.AsNotNull().Frame.Count,
				};
				if (newLabel != this.uActiveFrameRangeLabel__ItemNameOfAll) {
					newRange = this.ActiveFrameLabel.Find((value) => (value.Item1 == newLabel)).AsNotNull().Item2;
				}
				if (newRange != this.ActiveFrameRange) {
					await this.UpdateActiveFrameRange(newRange);
				}
			}
			return;
		}

		#endregion

		#region active frame speed

		public Floater uActiveFrameSpeedIcon_Opacity {
			get {
				if (!this.Activated) {
					return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(false);
				}
				return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public Boolean uActiveFrameSpeed_IsEnabled {
			get {
				if (!this.Activated) {
					return false;
				}
				return true;
			}
		}

		public DecimalFormatter uActiveFrameSpeed_NumberFormatter {
			get {
				return new () {
					IntegerDigits = 1,
					FractionDigits = 1,
					NumberRounder = new IncrementNumberRounder() {
						Increment = 0.1,
						RoundingAlgorithm = RoundingAlgorithm.RoundHalfUp,
					},
				};
			}
		}

		public Floater uActiveFrameSpeed_Value {
			get {
				if (!this.Activated) {
					return Floater.NaN;
				}
				return this.ActiveFrameSpeed.AsNotNull();
			}
		}

		public async void uActiveFrameSpeed_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.As<NumberBox>();
			if (!this.Activated) {
				return;
			}
			if (Floater.IsFinite(args.NewValue)) {
				var newValue = args.NewValue;
				if (newValue != this.ActiveFrameSpeed) {
					this.View.uSprite.FrameSpeed = newValue;
					this.ActiveFrameSpeed = newValue;
				}
			}
			this.NotifyPropertyChanged(
				nameof(this.uActiveFrameSpeed_Value)
			);
			return;
		}

		#endregion

		#region active progress

		public Boolean uActiveProgress_IsEnabled {
			get {
				if (!this.Activated) {
					return false;
				}
				return true;
			}
		}

		public Floater uActiveProgress_Minimum {
			get {
				if (!this.Activated) {
					return 0.0;
				}
				return this.ActiveFrameRange.Start + 1;
			}
		}

		public Floater uActiveProgress_Maximum {
			get {
				if (!this.Activated) {
					return 0.0;
				}
				return this.ActiveFrameRange.Start + this.ActiveFrameRange.Duration;
			}
		}

		public async void uActiveProgress_ValueChanged (
			Object                         sender,
			RangeBaseValueChangedEventArgs args
		) {
			var senders = sender.As<Slider>();
			if (!this.Activated) {
				return;
			}
			if (Floater.IsFinite(args.NewValue) && args.NewValue != 0.0) {
				GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
				if (this.uActiveProgress__Changeable) {
					this.View.uSprite.CurrentTime = TimeSpan.FromSeconds(args.NewValue - 1.0);
				}
			}
			return;
		}

		// ----------------

		public Boolean uActiveProgress__Changeable = false;

		public Boolean uActiveProgress__ChangingWhenPlaying = false;

		public async void uActiveProgress_PointerPressed (
			Object                 sender,
			PointerRoutedEventArgs args
		) {
			var senders = sender.As<Slider>();
			if (!this.Activated) {
				return;
			}
			GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
			this.uActiveProgress__Changeable = true;
			this.uActiveProgress__ChangingWhenPlaying = this.ActiveProgressState.AsNotNull();
			if (this.uActiveProgress__ChangingWhenPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
				this.ActiveProgressState = false;
				this.NotifyPropertyChanged(
					nameof(this.uActiveProgressStateIcon_Glyph)
				);
			}
			return;
		}

		public async void uActiveProgress_PointerReleased (
			Object                 sender,
			PointerRoutedEventArgs args
		) {
			var senders = sender.As<Slider>();
			if (!this.Activated) {
				return;
			}
			GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
			if (this.uActiveProgress__ChangingWhenPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Playing;
				this.ActiveProgressState = true;
				this.NotifyPropertyChanged(
					nameof(this.uActiveProgressStateIcon_Glyph)
				);
			}
			this.uActiveProgress__Changeable = false;
			this.uActiveProgress__ChangingWhenPlaying = false;
			return;
		}

		#endregion

		#region active progress state

		public Boolean uActiveProgressState_IsEnabled {
			get {
				if (!this.Activated) {
					return false;
				}
				return true;
			}
		}

		public async void uActiveProgressState_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (!this.Activated) {
				return;
			}
			GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
			var newState = this.View.uSprite.State != SpriteControl.StateType.Playing;
			this.View.uSprite.State = !newState ? SpriteControl.StateType.Paused : SpriteControl.StateType.Playing;
			this.ActiveProgressState = newState;
			this.NotifyPropertyChanged(
				nameof(this.uActiveProgressStateIcon_Glyph)
			);
			return;
		}

		// ----------------

		public String uActiveProgressStateIcon_Glyph {
			get {
				if (!this.Activated) {
					return FluentIconGlyph.Play;
				}
				return !this.ActiveProgressState.AsNotNull() ? FluentIconGlyph.Play : FluentIconGlyph.Pause;
			}
		}

		// ----------------

		public Boolean uActiveProgressPrevious_IsEnabled {
			get {
				if (!this.Activated) {
					return false;
				}
				return true;
			}
		}

		public async void uActiveProgressPrevious_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (!this.Activated) {
				return;
			}
			GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
			var newTime = this.View.uSprite.CurrentTime - TimeSpan.FromSeconds(1.0);
			var beginTime = TimeSpan.FromSeconds(this.ActiveFrameRange.Start);
			if (newTime < beginTime) {
				newTime = beginTime;
			}
			this.View.uSprite.CurrentTime = newTime;
			return;
		}

		// ----------------

		public Boolean uActiveProgressNext_IsEnabled {
			get {
				if (!this.Activated) {
					return false;
				}
				return true;
			}
		}

		public async void uActiveProgressNext_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<Button>();
			if (!this.Activated) {
				return;
			}
			GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
			var newTime = this.View.uSprite.CurrentTime + TimeSpan.FromSeconds(1.0);
			var endTime = TimeSpan.FromSeconds(this.ActiveFrameRange.Start + this.ActiveFrameRange.Duration - 1.0);
			if (newTime > endTime) {
				newTime = endTime;
			}
			this.View.uSprite.CurrentTime = newTime;
			return;
		}

		#endregion

		#region show boundary

		public Boolean uShowBoundary_IsChecked {
			get {
				return this.ShowBoundary;
			}
		}

		public async void uShowBoundary_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			this.ShowBoundary = senders.IsChecked.AsNotNull();
			this.View.uSprite.ShowBoundary = this.ShowBoundary;
			return;
		}

		#endregion

		#region filter rule

		public async void uFilterRule_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.As<SplitButton>();
			if (!this.Loaded) {
				return;
			}
			await this.ApplyFilterRule();
			return;
		}

		// ----------------

		public async void uImageFilterRuleText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.ImageFilterRule = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uImageFilterRuleText_Text)
			);
			return;
		}

		public String uImageFilterRuleText_Text {
			get {
				return this.ImageFilterRule;
			}
		}

		// ----------------

		public async void uSpriteFilterRuleText_LostFocus (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<TextBox>();
			this.SpriteFilterRule = senders.Text;
			this.NotifyPropertyChanged(
				nameof(this.uSpriteFilterRuleText_Text)
			);
			return;
		}

		public String uSpriteFilterRuleText_Text {
			get {
				return this.SpriteFilterRule;
			}
		}

		#endregion

		#region plant custom layer

		public Floater uPlantCustomLayerIcon_Opacity {
			get {
				if (!this.Loaded || this.PlantCustomLayerName.Count == 0) {
					return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(false);
				}
				return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public String uPlantCustomLayer__ItemNameOfNone { get; } = "\0";

		public Boolean uPlantCustomLayer_IsEnabled {
			get {
				if (!this.Loaded || this.PlantCustomLayerName.Count == 0) {
					return false;
				}
				return true;
			}
		}

		public List<String> uPlantCustomLayer_ItemsSource {
			get {
				if (!this.Loaded || this.PlantCustomLayerName.Count == 0) {
					return [];
				}
				return [..this.PlantCustomLayerName.Select((value) => (value["custom_".Length..])), this.uPlantCustomLayer__ItemNameOfNone];
			}
		}

		public String? uPlantCustomLayer_SelectedItem {
			get {
				if (!this.Loaded || this.PlantCustomLayerName.Count == 0) {
					return null;
				}
				var selectedItem = this.Animation.Sprite.Where((value, index) => (value.Name != null && this.PlantCustomLayerName.Contains(value.Name) && this.SpriteFilter[index])).ToList();
				return selectedItem.Count switch {
					0 => this.uPlantCustomLayer__ItemNameOfNone,
					1 => selectedItem[0].Name.AsNotNull()["custom_".Length..],
					_ => null,
				};
			}
		}

		public async void uPlantCustomLayer_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (!this.Loaded || this.PlantCustomLayerName.Count == 0) {
				return;
			}
			if (args.AddedItems.Count == 1) {
				var targetLayer = $"custom_{args.AddedItems[0].As<String>()}";
				await this.ApplyFilter(null, this.Animation.Sprite.Select((value) => (value.Name != null && this.PlantCustomLayerName.Contains(value.Name) ? value.Name == targetLayer : (Boolean?)null)).ToList());
				this.NotifyPropertyChanged(
					nameof(this.uPlantCustomLayer_SelectedItem)
				);
			}
			return;
		}

		#endregion

		#region zombie state layer

		public Floater uZombieStateLayerIcon_Opacity {
			get {
				if (!this.Loaded || this.ZombieStateLayerName.Count == 0) {
					return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(false);
				}
				return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public String uZombieStateLayer__ItemNameOfNone { get; } = "\0";

		public Boolean uZombieStateLayer_IsEnabled {
			get {
				if (!this.Loaded || this.ZombieStateLayerName.Count == 0) {
					return false;
				}
				return true;
			}
		}

		public List<String> uZombieStateLayer_ItemsSource {
			get {
				if (!this.Loaded || this.ZombieStateLayerName.Count == 0) {
					return [];
				}
				return [..this.ZombieStateLayerName, this.uZombieStateLayer__ItemNameOfNone];
			}
		}

		public String? uZombieStateLayer_SelectedItem {
			get {
				if (!this.Loaded || this.ZombieStateLayerName.Count == 0) { return null; }
				var selectedItem = this.Animation.Sprite.Where((value, index) => (value.Name != null && this.ZombieStateLayerName.Contains(value.Name) && this.SpriteFilter[index])).ToList();
				return selectedItem.Count switch {
					0 => this.uZombieStateLayer__ItemNameOfNone,
					1 => selectedItem[0].Name.AsNotNull(),
					_ => null,
				};
			}
		}

		public async void uZombieStateLayer_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.As<ComboBox>();
			if (!this.Loaded || this.ZombieStateLayerName.Count == 0) { return; }
			if (args.AddedItems.Count == 1) {
				var targetLayer = args.AddedItems[0].As<String>();
				await this.ApplyFilter(null, this.Animation.Sprite.Select((value) => (value.Name != null && this.ZombieStateLayerName.Contains(value.Name) ? value.Name == targetLayer : (Boolean?)null)).ToList());
				this.NotifyPropertyChanged(
					nameof(this.uZombieStateLayer_SelectedItem)
				);
			}
			return;
		}

		#endregion

		#region zombie ground swatch layer

		public Floater uZombieGroundSwatchLayerIcon_Opacity {
			get {
				if (!this.Loaded || this.ZombieGroundSwatchLayerName.Count == 0) { return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(false); }
				return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public Boolean uZombieGroundSwatchLayer_IsEnabled {
			get {
				if (!this.Loaded || this.ZombieGroundSwatchLayerName.Count == 0) { return false; }
				return true;
			}
		}

		public Boolean uZombieGroundSwatchLayer_IsChecked {
			get {
				if (!this.Loaded || this.ZombieGroundSwatchLayerName.Count == 0) { return false; }
				var selectedItem = this.Animation.Sprite.Where((value, index) => (value.Name != null && this.ZombieGroundSwatchLayerName.Contains(value.Name) && this.SpriteFilter[index])).ToList();
				return selectedItem.Count != 0;
			}
		}

		public async void uZombieGroundSwatchLayer_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			if (!this.Loaded || this.ZombieGroundSwatchLayerName.Count == 0) { return; }
			var newValue = senders.IsChecked.AsNotNull();
			await this.ApplyFilter(null, this.Animation.Sprite.Select((value) => (value.Name != null && this.ZombieGroundSwatchLayerName.Contains(value.Name) ? newValue : (Boolean?)null)).ToList());
			this.NotifyPropertyChanged(
				nameof(this.uZombieGroundSwatchLayer_IsChecked)
			);
			return;
		}

		#endregion

	}

	public class MainPageImageItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public Size Index { get; set; } = default!;

		#endregion

		#region view

		public String uTitle_Text {
			get {
				GF.AssertTest(this.Host.Loaded);
				var model = this.Host.Animation.Image[this.Index];
				return GameAnimationHelper.ParseImageFileName(model.Name);
			}
		}

		public String uDescription_Text {
			get {
				GF.AssertTest(this.Host.Loaded);
				var model = this.Host.Animation.Image[this.Index];
				var texture = this.Host.Texture.GetValueOrDefault(model.Name);
				return $"{model.Size?.Item1 ?? texture?.PixelWidth ?? 0} x {model.Size?.Item2 ?? texture?.PixelHeight ?? 0}";
			}
		}

		public UIElement uPreview_Content {
			get {
				GF.AssertTest(this.Host.Loaded);
				var model = this.Host.Animation.Image[this.Index];
				var texture = this.Host.Texture.GetValueOrDefault(model.Name);
				if (texture != null) {
					return new Image() {
						Width = 32, Height = 32,
						Source = texture,
					};
				}
				else {
					return new FontIcon() {
						Width = 32, Height = 32,
						FontSize = 20,
						Opacity = 0.7,
						Glyph = FluentIconGlyph.Error,
					};
				}
			}
		}

		// ----------------

		public Boolean uToggle_IsChecked {
			get {
				GF.AssertTest(this.Host.Loaded);
				return this.Host.ActiveTarget != null && this.Host.ActiveTarget.Item1 == false && this.Host.ActiveTarget.Item2 == this.Index;
			}
		}

		public async void uToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			GF.AssertTest(this.Host.Loaded);
			var needActivate = !(this.Host.ActiveTarget != null && this.Host.ActiveTarget.Item1 == false && this.Host.ActiveTarget.Item2 == this.Index);
			var newFrameSpeed = !this.Host.KeepSpeed ? null : this.Host.ActiveFrameSpeed;
			if (this.Host.Activated) {
				await this.Host.Deactivate();
			}
			if (needActivate) {
				await this.Host.Activate(new (false, this.Index), null, newFrameSpeed, null, null);
			}
			return;
		}

		#endregion

	}

	public class MainPageSpriteItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public Size Index { get; set; } = default!;

		#endregion

		#region view

		public String uTitle_Text {
			get {
				GF.AssertTest(this.Host.Loaded);
				var model = this.Host.Animation.Sprite[this.Index];
				return model.Name ?? "";
			}
		}

		public String uDescription_Text {
			get {
				GF.AssertTest(this.Host.Loaded);
				var model = this.Host.Animation.Sprite[this.Index];
				return $"{model.FrameRate ?? 0.0:F0} - {model.Frame.Count}";
			}
		}

		public UIElement uPreview_Content {
			get {
				GF.AssertTest(this.Host.Loaded);
				var model = this.Host.Animation.Sprite[this.Index];
				var texture = default(ImageSource?);
				if (model.Frame.Count == 1 && model.Frame[0].Append.Count == 1 && model.Frame[0].Change.Count == 1 && !model.Frame[0].Append[0].Sprite) {
					texture = this.Host.Texture.GetValueOrDefault(GameAnimationHelper.SelectImage(this.Host.Animation, (Size)model.Frame[0].Append[0].Resource).Name);
				}
				if (texture != null) {
					return new Image() {
						Width = 32, Height = 32,
						Source = texture,
					};
				}
				else {
					return new FontIcon() {
						Width = 32, Height = 32,
						FontSize = 20,
						Opacity = 0.7,
						Glyph = FluentIconGlyph.IOT,
					};
				}
			}
		}

		// ----------------

		public Boolean uToggle_IsChecked {
			get {
				GF.AssertTest(this.Host.Loaded);
				return this.Host.ActiveTarget != null && this.Host.ActiveTarget.Item1 == true && this.Host.ActiveTarget.Item2 == this.Index;
			}
		}

		public async void uToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			GF.AssertTest(this.Host.Loaded);
			var needActivate = !(this.Host.ActiveTarget != null && this.Host.ActiveTarget.Item1 == true && this.Host.ActiveTarget.Item2 == this.Index);
			var newFrameSpeed = !this.Host.KeepSpeed ? null : this.Host.ActiveFrameSpeed;
			if (this.Host.Activated) {
				await this.Host.Deactivate();
			}
			if (needActivate) {
				await this.Host.Activate(new (true, this.Index), null, newFrameSpeed, null, null);
			}
			return;
		}

		#endregion

	}

	public class MainPageMainSpriteItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public Size? Index { get; set; } = default!;

		#endregion

		#region view

		public Floater uRoot_Opacity {
			get {
				if (this.Index == null) {
					return 0.0;
				}
				else {
					GF.AssertTest(this.Host.Loaded);
					return 1.0;
				}
			}
		}

		// ----------------

		public String uTitle_Text {
			get {
				if (this.Index == null) {
					return "";
				}
				else {
					GF.AssertTest(this.Host.Loaded);
					var model = this.Host.Animation.MainSprite!;
					return model.Name ?? "";
				}
			}
		}

		public String uDescription_Text {
			get {
				if (this.Index == null) {
					return "";
				}
				else {
					GF.AssertTest(this.Host.Loaded);
					var model = this.Host.Animation.MainSprite!;
					return $"{model.FrameRate ?? 0.0:F0} - {model.Frame.Count}";
				}
			}
		}

		public UIElement uPreview_Content {
			get {
				return new FontIcon() {
					Width = 32, Height = 32,
					FontSize = 20,
					Opacity = 0.7,
					Glyph = FluentIconGlyph.IOT,
				};
			}
		}

		// ----------------

		public Boolean uToggle_IsChecked {
			get {
				if (this.Index == null) {
					return false;
				}
				else {
					GF.AssertTest(this.Host.Loaded);
					return this.Host.ActiveTarget != null && this.Host.ActiveTarget.Item1 == true && this.Host.ActiveTarget.Item2 == this.Index;
				}
			}
		}

		public async void uToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.As<ToggleButton>();
			GF.AssertTest(this.Host.Loaded);
			var needActivate = !(this.Host.ActiveTarget != null && this.Host.ActiveTarget.Item1 == true && this.Host.ActiveTarget.Item2 == this.Index);
			var newFrameSpeed = !this.Host.KeepSpeed ? null : this.Host.ActiveFrameSpeed;
			if (this.Host.Activated) {
				await this.Host.Deactivate();
			}
			if (needActivate) {
				await this.Host.Activate(new (true, this.Index.AsNotNull()), null, newFrameSpeed, null, null);
			}
			return;
		}

		#endregion

	}

}
