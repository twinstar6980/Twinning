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
			this.Controller.ApplyOption(args.Parameter.AsClass<List<String>>());
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		private MainPageController Controller { get; }

		#endregion

		#region module page

		public async Task<List<String>> ModulePageCollectOption (
		) {
			return await this.Controller.CollectOption();
		}

		public async Task<Boolean> ModulePageRequestClose (
		) {
			return await this.Controller.RequestClose();
		}

		#endregion

	}

	public class MainPageController : CustomController {

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

		public Boolean RemainFrameRate { get; set; } = default!;

		// ----------------

		public String? AnimationFile { get; set; } = null;

		public String? ImageDirectory { get; set; } = null;

		// ----------------

		public GameAnimationModel.Animation? Animation { get; set; } = null;

		public List<BitmapSource?>? ImageSource { get; set; } = null;

		public List<Boolean>? ImageFilter { get; set; } = null;

		public List<Boolean>? SpriteFilter { get; set; } = null;

		// ----------------

		public Size? WorkingSpriteIndex { get; set; } = null;

		public GameAnimationHelper.FrameRange? WorkingSpriteFrameRange { get; set; } = null;

		public List<Tuple<String, GameAnimationHelper.FrameRange>>? WorkingSpriteFrameRangeLabelInformation { get; set; } = null;

		public Floater? WorkingSpriteFrameRate { get; set; } = null;

		public Boolean? WorkingSpriteState { get; set; } = null;

		// ----------------

		public Boolean ShowSpriteBoundary { get; set; } = default!;

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
			this.RemainFrameRate = App.Setting.Data.AnimationViewer.RemainFrameRate;
			this.ShowSpriteBoundary = App.Setting.Data.AnimationViewer.ShowSpriteBoundary;
			this.ImageFilterRule = App.Setting.Data.AnimationViewer.ImageFilterRule;
			this.SpriteFilterRule = App.Setting.Data.AnimationViewer.SpriteFilterRule;
			this.View.uSprite.HoldEnd = true;
			this.View.uSprite.Repeat = true;
			this.View.uSprite.ShowBoundary = this.ShowSpriteBoundary;
			this.View.uWorkingSpriteFrameProgress.AddHandler(UIElement.PointerPressedEvent, new PointerEventHandler(this.uWorkingSpriteFrameProgress_PointerPressed), true);
			this.View.uWorkingSpriteFrameProgress.AddHandler(UIElement.PointerReleasedEvent, new PointerEventHandler(this.uWorkingSpriteFrameProgress_PointerReleased), true);
			return;
		}

		public async void ApplyOption (
			List<String> optionView
		) {
			await ControlHelper.WaitUntilLoaded(this.View);
			var optionImmediateSelect = default(Boolean?);
			var optionAutomaticPlay = default(Boolean?);
			var optionRepeatPlay = default(Boolean?);
			var optionRemainFrameRate = default(Boolean?);
			var optionShowSpriteBoundary = default(Boolean?);
			var optionImageFilterRule = default(String?);
			var optionSpriteFilterRule = default(String?);
			var optionAnimationFile = default(String?);
			var optionImageDirectory = default(String?);
			var optionWorkingSpriteIndex = default(Integer?);
			var optionWorkingSpriteFrameRange = default(Tuple<Integer, Integer>?);
			var optionWorkingSpriteFrameRate = default(Floater?);
			var optionWorkingSpriteState = default(Boolean?);
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
				if (option.Check("-RemainFrameRate")) {
					optionRemainFrameRate = option.NextBoolean();
				}
				if (option.Check("-ShowSpriteBoundary")) {
					optionShowSpriteBoundary = option.NextBoolean();
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
				if (option.Check("-ImageDirectory")) {
					optionImageDirectory = option.NextString();
				}
				if (option.Check("-WorkingSpriteIndex")) {
					optionWorkingSpriteIndex = option.NextInteger();
				}
				if (option.Check("-WorkingSpriteFrameRange")) {
					optionWorkingSpriteFrameRange = new (
						option.NextInteger(),
						option.NextInteger()
					);
				}
				if (option.Check("-WorkingSpriteFrameRate")) {
					optionWorkingSpriteFrameRate = option.NextFloater();
				}
				if (option.Check("-WorkingSpriteState")) {
					optionWorkingSpriteState = option.NextBoolean();
				}
				if (!option.Done()) {
					throw new ($"Too many option : '{String.Join(' ', option.NextStringList())}'.");
				}
			}
			catch (Exception e) {
				App.MainWindow.PublishNotification(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			if (optionImmediateSelect is not null) {
				this.ImmediateSelect = optionImmediateSelect.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uImmediateSelect_IsChecked)
				);
			}
			if (optionAutomaticPlay is not null) {
				this.AutomaticPlay = optionAutomaticPlay.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uAutomaticPlay_IsChecked)
				);
			}
			if (optionRepeatPlay is not null) {
				this.RepeatPlay = optionRepeatPlay.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uRepeatPlay_IsChecked)
				);
			}
			if (optionRemainFrameRate is not null) {
				this.RemainFrameRate = optionRemainFrameRate.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uRemainFrameRate_IsChecked)
				);
			}
			if (optionShowSpriteBoundary is not null) {
				this.ShowSpriteBoundary = optionShowSpriteBoundary.AsNotNull();
				this.NotifyPropertyChanged(
					nameof(this.uShowSpriteBoundary_IsChecked)
				);
			}
			if (optionImageFilterRule is not null) {
				this.ImageFilterRule = optionImageFilterRule;
				this.NotifyPropertyChanged(
					nameof(this.uImageFilterRuleText_Text)
				);
			}
			if (optionSpriteFilterRule is not null) {
				this.SpriteFilterRule = optionSpriteFilterRule;
				this.NotifyPropertyChanged(
					nameof(this.uSpriteFilterRuleText_Text)
				);
			}
			if (optionAnimationFile is not null) {
				await this.ApplyLoad(
					optionAnimationFile,
					optionImageDirectory,
					optionWorkingSpriteIndex is null ? null : (Size)optionWorkingSpriteIndex,
					optionWorkingSpriteFrameRange is null ? null : new () { Start = (Size)optionWorkingSpriteFrameRange.Item1, Duration = (Size)optionWorkingSpriteFrameRange.Item2 },
					optionWorkingSpriteFrameRate,
					optionWorkingSpriteState
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
			if (option.Check("-RemainFrameRate")) {
				option.NextBoolean(this.RemainFrameRate);
			}
			if (option.Check("-ShowSpriteBoundary")) {
				option.NextBoolean(this.ShowSpriteBoundary);
			}
			if (option.Check("-ImageFilterRule")) {
				option.NextString(this.ImageFilterRule);
			}
			if (option.Check("-SpriteFilterRule")) {
				option.NextString(this.SpriteFilterRule);
			}
			if (option.Check("-AnimationFile", this.AnimationFile is not null)) {
				option.NextString(this.AnimationFile.AsNotNull());
			}
			if (option.Check("-ImageDirectory", this.ImageDirectory is not null)) {
				option.NextString(this.ImageDirectory.AsNotNull());
			}
			if (option.Check("-WorkingSpriteIndex", this.WorkingSpriteIndex is not null)) {
				option.NextInteger(this.WorkingSpriteIndex.AsNotNull());
			}
			if (option.Check("-WorkingSpriteFrameRange", this.WorkingSpriteFrameRange is not null)) {
				option.NextInteger(this.WorkingSpriteFrameRange.AsNotNull().Start);
				option.NextInteger(this.WorkingSpriteFrameRange.AsNotNull().Duration);
			}
			if (option.Check("-WorkingSpriteFrameRate", this.WorkingSpriteFrameRate is not null)) {
				option.NextFloater(this.WorkingSpriteFrameRate.AsNotNull());
			}
			if (option.Check("-WorkingSpriteState", this.WorkingSpriteState is not null)) {
				option.NextBoolean(this.WorkingSpriteState.AsNotNull());
			}
			return option.Done();
		}

		public async Task<Boolean> RequestClose (
		) {
			return true;
		}

		#endregion

		#region load & work

		[MemberNotNullWhen(true, nameof(MainPageController.AnimationFile))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageDirectory))]
		[MemberNotNullWhen(true, nameof(MainPageController.Animation))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageSource))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.SpriteFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.PlantCustomLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieStateLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieGroundSwatchLayerName))]
		public Boolean Loaded {
			get {
				return this.Animation is not null;
			}
		}

		[MemberNotNullWhen(true, nameof(MainPageController.AnimationFile))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageDirectory))]
		[MemberNotNullWhen(true, nameof(MainPageController.Animation))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageSource))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.SpriteFilter))]
		[MemberNotNullWhen(true, nameof(MainPageController.PlantCustomLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieStateLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.ZombieGroundSwatchLayerName))]
		[MemberNotNullWhen(true, nameof(MainPageController.WorkingSpriteIndex))]
		[MemberNotNullWhen(true, nameof(MainPageController.WorkingSpriteFrameRange))]
		[MemberNotNullWhen(true, nameof(MainPageController.WorkingSpriteFrameRangeLabelInformation))]
		[MemberNotNullWhen(true, nameof(MainPageController.WorkingSpriteFrameRate))]
		[MemberNotNullWhen(true, nameof(MainPageController.WorkingSpriteState))]
		public Boolean Working {
			get {
				return this.Loaded && this.WorkingSpriteIndex is not null;
			}
		}

		// ----------------

		public async Task Load (
			String animationFile,
			String imageDirectory
		) {
			GF.AssertTest(!this.Loaded && !this.Working);
			var animationData = default(GameAnimationModel.Animation);
			var imageSourceData = default(List<BitmapSource?>);
			try {
				animationData = await GameAnimationHelper.LoadAnimation(animationFile);
				imageSourceData = await GameAnimationHelper.LoadImageSource(imageDirectory, animationData);
			}
			catch (Exception e) {
				App.MainWindow.PublishNotification(InfoBarSeverity.Error, "Failed to load animation.", e.ToString());
				return;
			}
			this.AnimationFile = animationFile;
			this.ImageDirectory = imageDirectory;
			this.Animation = animationData;
			this.ImageSource = imageSourceData;
			this.ImageFilter = Enumerable.Range(0, this.Animation.Image.Count).Select((index) => (false)).ToList();
			this.SpriteFilter = Enumerable.Range(0, this.Animation.Sprite.Count).Select((index) => (false)).ToList();
			this.PlantCustomLayerName = this.Animation.Sprite.Where((value) => (value.Name is not null)).Select((value) => (value.Name.AsNotNull())).Where((value) => (value.StartsWith("custom_"))).ToList();
			this.ZombieStateLayerName = this.Animation.Sprite.Where((value) => (value.Name is not null)).Select((value) => (value.Name.AsNotNull())).Where((value) => (value == "ink" || value == "butter")).ToList();
			this.ZombieGroundSwatchLayerName = this.Animation.Sprite.Where((value) => (value.Name is not null)).Select((value) => (value.Name.AsNotNull())).Where((value) => (value == "ground_swatch" || value == "ground_swatch_plane")).ToList();
			this.NotifyPropertyChanged(
				nameof(this.uClearSource_IsEnabled),
				nameof(this.uAnimationFile_IsEnabled),
				nameof(this.uAnimationFile_Text),
				nameof(this.uImageDirectory_IsEnabled),
				nameof(this.uImageDirectory_Text),
				nameof(this.uImageDirectoryPick_IsEnabled),
				nameof(this.uImageList_IsEnabled),
				nameof(this.uImageList_ItemsSource),
				nameof(this.uSpriteList_IsEnabled),
				nameof(this.uSpriteList_ItemsSource),
				nameof(this.uMainSpriteList_IsEnabled),
				nameof(this.uMainSpriteList_ItemsSource)
			);
			this.View.uImageList.SelectAll();
			this.View.uSpriteList.SelectAll();
			if (this.Animation.MainSprite is not null) {
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
			GF.AssertTest(this.Loaded && !this.Working);
			this.View.uImageList.DeselectRange(new (0, (USize)this.Animation.Image.Count));
			this.View.uSpriteList.DeselectRange(new (0, (USize)this.Animation.Sprite.Count));
			if (this.Animation.MainSprite is not null) {
				this.View.uMainSpriteList.DeselectRange(new (0, 1));
			}
			this.AnimationFile = null;
			this.ImageDirectory = null;
			this.Animation = null;
			this.ImageSource = null;
			this.ImageFilter = null;
			this.SpriteFilter = null;
			this.PlantCustomLayerName = null;
			this.ZombieStateLayerName = null;
			this.ZombieGroundSwatchLayerName = null;
			this.NotifyPropertyChanged(
				nameof(this.uClearSource_IsEnabled),
				nameof(this.uAnimationFile_IsEnabled),
				nameof(this.uAnimationFile_Text),
				nameof(this.uImageDirectory_IsEnabled),
				nameof(this.uImageDirectory_Text),
				nameof(this.uImageDirectoryPick_IsEnabled),
				nameof(this.uImageList_IsEnabled),
				nameof(this.uImageList_ItemsSource),
				nameof(this.uSpriteList_IsEnabled),
				nameof(this.uSpriteList_ItemsSource),
				nameof(this.uMainSpriteList_IsEnabled),
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

		public async Task LoadWorkingSprite (
			Size                            index,
			GameAnimationHelper.FrameRange? frameRange,
			Floater?                        frameRate,
			Boolean?                        initialState,
			TimeSpan?                       initialTime
		) {
			GF.AssertTest(this.Loaded && !this.Working);
			var workingSprite = GameAnimationHelper.SelectSprite(this.Animation, index);
			this.WorkingSpriteIndex = index;
			this.WorkingSpriteFrameRange = frameRange ?? new () { Start = 0, Duration = workingSprite.Frame.Count };
			this.WorkingSpriteFrameRangeLabelInformation = [];
			var currentFrameLabel = new List<Tuple<String, Size>>();
			for (var frameIndex = 0; frameIndex < workingSprite.Frame.Count; frameIndex++) {
				var frame = workingSprite.Frame[frameIndex];
				if (frame.Label is not null) {
					currentFrameLabel.Add(new (frame.Label, frameIndex));
				}
				if (frame.Stop) {
					foreach (var item in currentFrameLabel) {
						this.WorkingSpriteFrameRangeLabelInformation.Add(new (item.Item1, new () {
							Start = item.Item2,
							Duration = frameIndex - item.Item2 + 1,
						}));
					}
					currentFrameLabel.Clear();
				}
			}
			this.WorkingSpriteFrameRate = frameRate ?? workingSprite.FrameRate ?? (Floater)this.Animation.FrameRate;
			this.WorkingSpriteState = initialState ?? this.AutomaticPlay;
			this.View.uSprite.Load(this.Animation, this.ImageSource, this.ImageFilter, this.SpriteFilter, index);
			GF.AssertTest(this.View.uSprite.Loaded);
			var sliderAnimation = new ObjectAnimationUsingKeyFrames();
			for (var frameIndex = 0; frameIndex < workingSprite.Frame.Count; frameIndex++) {
				sliderAnimation.KeyFrames.Add(
					new DiscreteObjectKeyFrame() {
						KeyTime = KeyTime.FromTimeSpan(TimeSpan.FromSeconds(frameIndex)),
						Value = (Floater)(frameIndex + 1),
					}
				);
			}
			Storyboard.SetTargetProperty(sliderAnimation, "Value");
			var sliderStoryboard = new Storyboard() {
				Duration = new (TimeSpan.FromSeconds(workingSprite.Frame.Count)),
				RepeatBehavior = RepeatBehavior.Forever,
				Children = {
					sliderAnimation,
				},
			};
			Storyboard.SetTarget(sliderStoryboard, this.View.uWorkingSpriteFrameProgress);
			this.View.uSprite.Storyboard.Children.Add(sliderStoryboard);
			this.View.uSprite.Storyboard.Completed += (sender, o) => {
				if (!this.RepeatPlay) {
					this.View.uSprite.State = SpriteControl.StateType.Paused;
					this.WorkingSpriteState = false;
					this.NotifyPropertyChanged(
						nameof(this.uWorkingSpriteStateIcon_Glyph)
					);
				}
			};
			this.View.uSprite.FrameRange = this.WorkingSpriteFrameRange;
			this.View.uSprite.Speed = this.WorkingSpriteFrameRate.AsNotNull();
			this.View.uSprite.State = !this.WorkingSpriteState.AsNotNull() ? SpriteControl.StateType.Paused : SpriteControl.StateType.Playing;
			this.View.uSprite.CurrentTime = initialTime ?? TimeSpan.FromSeconds(this.WorkingSpriteFrameRange.Start);
			this.NotifyPropertyChanged(
				nameof(this.uWorkingSpriteFrameRangeIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRangeBegin_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeBegin_Minimum),
				nameof(this.uWorkingSpriteFrameRangeBegin_Maximum),
				nameof(this.uWorkingSpriteFrameRangeBegin_Value),
				nameof(this.uWorkingSpriteFrameRangeEnd_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeEnd_Minimum),
				nameof(this.uWorkingSpriteFrameRangeEnd_Maximum),
				nameof(this.uWorkingSpriteFrameRangeEnd_Value),
				nameof(this.uWorkingSpriteFrameRangeLabelIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRangeLabel_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeLabel_ItemsSource),
				nameof(this.uWorkingSpriteFrameRangeLabel_SelectedItem),
				nameof(this.uWorkingSpriteFrameRateIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRate_IsEnabled),
				nameof(this.uWorkingSpriteFrameRate_Value),
				nameof(this.uWorkingSpriteFrameProgress_IsEnabled),
				nameof(this.uWorkingSpriteFrameProgress_Minimum),
				nameof(this.uWorkingSpriteFrameProgress_Maximum),
				nameof(this.uWorkingSpriteState_IsEnabled),
				nameof(this.uWorkingSpriteStateIcon_Glyph),
				nameof(this.uWorkingSpritePrevious_IsEnabled),
				nameof(this.uWorkingSpriteNext_IsEnabled)
			);
			foreach (var item in this.View.uSpriteList.ItemsSource.AsClass<List<MainPageSpriteItemController>>()) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			foreach (var item in this.View.uMainSpriteList.ItemsSource.AsClass<List<MainPageMainSpriteItemController>>()) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			return;
		}

		public async Task UnloadWorkingSprite (
		) {
			GF.AssertTest(this.Loaded && this.Working);
			this.WorkingSpriteIndex = null;
			this.WorkingSpriteFrameRange = null;
			this.WorkingSpriteFrameRangeLabelInformation = null;
			this.WorkingSpriteFrameRate = null;
			this.WorkingSpriteState = null;
			this.NotifyPropertyChanged(
				nameof(this.uWorkingSpriteFrameRangeIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRangeBegin_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeBegin_Minimum),
				nameof(this.uWorkingSpriteFrameRangeBegin_Maximum),
				nameof(this.uWorkingSpriteFrameRangeBegin_Value),
				nameof(this.uWorkingSpriteFrameRangeEnd_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeEnd_Minimum),
				nameof(this.uWorkingSpriteFrameRangeEnd_Maximum),
				nameof(this.uWorkingSpriteFrameRangeEnd_Value),
				nameof(this.uWorkingSpriteFrameRangeLabelIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRangeLabel_IsEnabled),
				nameof(this.uWorkingSpriteFrameRangeLabel_ItemsSource),
				nameof(this.uWorkingSpriteFrameRangeLabel_SelectedItem),
				nameof(this.uWorkingSpriteFrameRateIcon_Opacity),
				nameof(this.uWorkingSpriteFrameRate_IsEnabled),
				nameof(this.uWorkingSpriteFrameRate_Value),
				nameof(this.uWorkingSpriteFrameProgress_IsEnabled),
				nameof(this.uWorkingSpriteFrameProgress_Minimum),
				nameof(this.uWorkingSpriteFrameProgress_Maximum),
				nameof(this.uWorkingSpriteState_IsEnabled),
				nameof(this.uWorkingSpriteStateIcon_Glyph),
				nameof(this.uWorkingSpritePrevious_IsEnabled),
				nameof(this.uWorkingSpriteNext_IsEnabled)
			);
			foreach (var item in this.View.uSpriteList.ItemsSource.AsClass<List<MainPageSpriteItemController>>()) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			foreach (var item in this.View.uMainSpriteList.ItemsSource.AsClass<List<MainPageMainSpriteItemController>>()) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			this.View.uSprite.Unload();
			return;
		}

		public async Task UpdateWorkingSpriteFrameRange (
			GameAnimationHelper.FrameRange frameRange
		) {
			GF.AssertTest(this.Loaded && this.Working);
			this.WorkingSpriteFrameRange = frameRange;
			this.View.uSprite.FrameRange = frameRange;
			this.View.uSprite.CurrentTime = TimeSpan.FromSeconds(frameRange.Start);
			this.NotifyPropertyChanged(
				nameof(this.uWorkingSpriteFrameRangeBegin_Value),
				nameof(this.uWorkingSpriteFrameRangeEnd_Value),
				nameof(this.uWorkingSpriteFrameRangeLabel_SelectedItem),
				nameof(this.uWorkingSpriteFrameProgress_Minimum),
				nameof(this.uWorkingSpriteFrameProgress_Maximum),
				nameof(this.uWorkingSpriteStateIcon_Glyph)
			);
			return;
		}

		// ----------------

		public async Task ApplyLoad (
			String                          animationFile,
			String?                         imageDirectory,
			Size?                           spriteIndex,
			GameAnimationHelper.FrameRange? spriteFrameRange,
			Floater?                        spriteFrameRate,
			Boolean?                        spriteInitialState
		) {
			imageDirectory ??= StorageHelper.Parent(animationFile).AsNotNull();
			if (this.Loaded) {
				if (this.Working) {
					await this.UnloadWorkingSprite();
				}
				await this.Unload();
			}
			await this.Load(animationFile, imageDirectory);
			if (this.Loaded) {
				await this.ApplyFilterRule();
				if (this.ImmediateSelect && this.Animation.MainSprite is not null) {
					await this.LoadWorkingSprite(spriteIndex ?? this.Animation.Sprite.Count, spriteFrameRange, spriteFrameRate, spriteInitialState, null);
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
			var spriteIndex = this.WorkingSpriteIndex;
			var frameRange = this.WorkingSpriteFrameRange;
			var frameRate = this.WorkingSpriteFrameRate;
			var currentState = this.WorkingSpriteState;
			var currentTime = !this.Working ? (TimeSpan?)null : this.View.uSprite.CurrentTime;
			if (spriteIndex is not null) {
				await this.UnloadWorkingSprite();
			}
			this.SuppressFilterListSelectionChanged = true;
			if (imageFilter is not null) {
				for (var index = 0; index < imageFilter.Count; index++) {
					var state = imageFilter[index];
					if (state is null || state.AsNotNull() == this.ImageFilter[index]) {
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
			if (spriteFilter is not null) {
				for (var index = 0; index < spriteFilter.Count; index++) {
					var state = spriteFilter[index];
					if (state is null || state.AsNotNull() == this.SpriteFilter[index]) {
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
			if (spriteIndex is not null) {
				await this.LoadWorkingSprite(spriteIndex.AsNotNull(), frameRange, frameRate, currentState, currentTime);
			}
			this.SuppressApplyFilterChanged = false;
			return;
		}

		public async Task ApplyFilterRule (
		) {
			GF.AssertTest(this.Loaded);
			await this.ApplyFilter(
				this.Animation.Image.Select((value) => (this.ImageFilterRule.Length != 0 && Regex.IsMatch(GameAnimationHelper.ParseImageFileName(value.Name), this.ImageFilterRule) ? false : (Boolean?)null)).ToList(),
				this.Animation.Sprite.Select((value) => (value.Name is not null && this.SpriteFilterRule.Length != 0 && Regex.IsMatch(value.Name, this.SpriteFilterRule) ? false : (Boolean?)null)).ToList()
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
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.AcceptedOperation = Windows.ApplicationModel.DataTransfer.DataPackageOperation.Link;
			}
			return;
		}

		public async void uPage_Drop (
			Object        sender,
			DragEventArgs args
		) {
			var senders = sender.AsClass<Page>();
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.Handled = true;
				var item = await args.DataView.GetStorageItemsAsync();
				if (item.Count != 1) {
					App.MainWindow.PublishNotification(InfoBarSeverity.Error, "Source is multiply.", "");
					return;
				}
				var animationFile = StorageHelper.Regularize(item[0].Path);
				if (!StorageHelper.ExistFile(animationFile)) {
					App.MainWindow.PublishNotification(InfoBarSeverity.Error, "Source is not a file.", "");
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
			var senders = sender.AsClass<ScrollViewer>();
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
			var senders = sender.AsClass<ToggleButton>();
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
			var senders = sender.AsClass<ToggleButton>();
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
			var senders = sender.AsClass<ToggleButton>();
			this.RepeatPlay = senders.IsChecked.AsNotNull();
			this.View.uSprite.Repeat = this.RepeatPlay;
			return;
		}

		// ----------------

		public Boolean uRemainFrameRate_IsChecked {
			get {
				return this.RemainFrameRate;
			}
		}

		public async void uRemainFrameRate_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.RemainFrameRate = senders.IsChecked.AsNotNull();
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
			var senders = sender.AsClass<Button>();
			if (!this.Loaded) {
				return;
			}
			if (this.Working) {
				await this.UnloadWorkingSprite();
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
			var senders = sender.AsClass<Button>();
			var isPlaying = this.Working && this.View.uSprite.State == SpriteControl.StateType.Playing;
			if (isPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
			}
			var animationFile = await StorageHelper.PickOpenFile(WindowHelper.Find(this.View), $"{nameof(AnimationViewer)}.AnimationFile");
			if (animationFile is not null) {
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

		public Boolean uImageDirectory_IsEnabled {
			get {
				if (!this.Loaded) {
					return false;
				}
				return true;
			}
		}

		public String uImageDirectory_Text {
			get {
				if (!this.Loaded) {
					return "";
				}
				return this.ImageDirectory;
			}
		}

		public Boolean uImageDirectoryPick_IsEnabled {
			get {
				if (!this.Loaded) {
					return false;
				}
				return true;
			}
		}

		public async void uImageDirectoryPick_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			if (!this.Loaded) {
				return;
			}
			var isPlaying = this.Working && this.View.uSprite.State == SpriteControl.StateType.Playing;
			if (isPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
			}
			var imageDirectory = await StorageHelper.PickOpenDirectory(WindowHelper.Find(this.View), $"{nameof(AnimationViewer)}.ImageDirectory");
			if (imageDirectory is not null) {
				await this.ApplyLoad(this.AnimationFile, imageDirectory, null, null, null, null);
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

		public Boolean uImageList_IsEnabled {
			get {
				if (!this.Loaded) {
					return false;
				}
				return true;
			}
		}

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
			var senders = sender.AsClass<ListView>();
			if (!this.Loaded) {
				return;
			}
			if (this.SuppressFilterListSelectionChanged) { return; }
			foreach (var item in args.AddedItems.Select(GF.AsClass<MainPageImageItemController>)) {
				this.ImageFilter[item.Index] = true;
			}
			foreach (var item in args.RemovedItems.Select(GF.AsClass<MainPageImageItemController>)) {
				this.ImageFilter[item.Index] = false;
			}
			this.SuppressApplyFilterChanged = true;
			this.NotifyPropertyChanged(
				nameof(this.uPlantCustomLayer_SelectedItem),
				nameof(this.uZombieStateLayer_SelectedItem),
				nameof(this.uZombieGroundSwatchLayer_IsChecked)
			);
			this.SuppressApplyFilterChanged = false;
			if (this.Working && (args.AddedItems.Count != 0 || args.RemovedItems.Count != 0)) {
				var spriteIndex = this.WorkingSpriteIndex;
				var frameRange = this.WorkingSpriteFrameRange;
				var frameRate = this.WorkingSpriteFrameRate;
				var currentState = this.WorkingSpriteState;
				var currentTime = this.View.uSprite.CurrentTime;
				await this.UnloadWorkingSprite();
				await this.LoadWorkingSprite(spriteIndex.AsNotNull(), frameRange, frameRate, currentState, currentTime);
			}
			return;
		}

		// ----------------

		public Boolean uSpriteList_IsEnabled {
			get {
				if (!this.Loaded) {
					return false;
				}
				return true;
			}
		}

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
			var senders = sender.AsClass<ListView>();
			if (!this.Loaded) {
				return;
			}
			if (this.SuppressFilterListSelectionChanged) { return; }
			foreach (var item in args.AddedItems.Select(GF.AsClass<MainPageSpriteItemController>)) {
				this.SpriteFilter[item.Index] = true;
			}
			foreach (var item in args.RemovedItems.Select(GF.AsClass<MainPageSpriteItemController>)) {
				this.SpriteFilter[item.Index] = false;
			}
			this.SuppressApplyFilterChanged = true;
			this.NotifyPropertyChanged(
				nameof(this.uPlantCustomLayer_SelectedItem),
				nameof(this.uZombieStateLayer_SelectedItem),
				nameof(this.uZombieGroundSwatchLayer_IsChecked)
			);
			this.SuppressApplyFilterChanged = false;
			if (this.Working && (args.AddedItems.Count != 0 || args.RemovedItems.Count != 0)) {
				var spriteIndex = this.WorkingSpriteIndex;
				var frameRange = this.WorkingSpriteFrameRange;
				var frameRate = this.WorkingSpriteFrameRate;
				var currentState = this.WorkingSpriteState;
				var currentTime = this.View.uSprite.CurrentTime;
				await this.UnloadWorkingSprite();
				await this.LoadWorkingSprite(spriteIndex.AsNotNull(), frameRange, frameRate, currentState, currentTime);
			}
			return;
		}

		// ----------------

		public Boolean uMainSpriteList_IsEnabled {
			get {
				if (!this.Loaded) {
					return false;
				}
				return this.Animation.MainSprite is not null;
			}
		}

		public List<MainPageMainSpriteItemController> uMainSpriteList_ItemsSource {
			get {
				if (!this.Loaded) {
					return [new () { Host = this, Index = null }];
				}
				return [new () { Host = this, Index = this.Animation.MainSprite is null ? null : this.Animation.Sprite.Count }];
			}
		}

		public async void uMainSpriteList_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ListView>();
			if (!this.Loaded) {
				return;
			}
			if (senders.SelectedItems.Count == 0) {
				senders.SelectAll();
			}
			return;
		}

		#endregion

		#region working frame range

		public Floater uWorkingSpriteFrameRangeIcon_Opacity {
			get {
				if (!this.Working) {
					return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(false);
				}
				return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public Boolean uWorkingSpriteFrameRangeBegin_IsEnabled {
			get {
				if (!this.Working) {
					return false;
				}
				return true;
			}
		}

		public DecimalFormatter uWorkingSpriteFrameRangeBegin_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uWorkingSpriteFrameRangeBegin_Minimum {
			get {
				if (!this.Working) {
					return Floater.NaN;
				}
				return 1.0;
			}
		}

		public Floater uWorkingSpriteFrameRangeBegin_Maximum {
			get {
				if (!this.Working) {
					return Floater.NaN;
				}
				return GameAnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.AsNotNull()).Frame.Count;
			}
		}

		public Floater uWorkingSpriteFrameRangeBegin_Value {
			get {
				if (!this.Working) {
					return Floater.NaN;
				}
				return this.WorkingSpriteFrameRange.Start + 1;
			}
		}

		public async void uWorkingSpriteFrameRangeBegin_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (!this.Working) {
				return;
			}
			if (!Floater.IsNaN(args.NewValue)) {
				var newBegin = (Size)args.NewValue - 1;
				var newRange = new GameAnimationHelper.FrameRange() {
					Start = newBegin,
					Duration = this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration - newBegin,
				};
				if (newRange.Start >= this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration) {
					newRange.Duration = 1;
				}
				if (newRange != this.WorkingSpriteFrameRange) {
					await this.UpdateWorkingSpriteFrameRange(newRange);
				}
			}
			this.NotifyPropertyChanged(
				nameof(this.uWorkingSpriteFrameRangeBegin_Value)
			);
			return;
		}

		// ----------------

		public Boolean uWorkingSpriteFrameRangeEnd_IsEnabled {
			get {
				if (!this.Working) {
					return false;
				}
				return true;
			}
		}

		public DecimalFormatter uWorkingSpriteFrameRangeEnd_NumberFormatter {
			get {
				return new () { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uWorkingSpriteFrameRangeEnd_Minimum {
			get {
				if (!this.Working) {
					return Floater.NaN;
				}
				return 1.0;
			}
		}

		public Floater uWorkingSpriteFrameRangeEnd_Maximum {
			get {
				if (!this.Working) {
					return Floater.NaN;
				}
				return GameAnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.AsNotNull()).Frame.Count;
			}
		}

		public Floater uWorkingSpriteFrameRangeEnd_Value {
			get {
				if (!this.Working) {
					return Floater.NaN;
				}
				return this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration;
			}
		}

		public async void uWorkingSpriteFrameRangeEnd_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (!this.Working) {
				return;
			}
			if (!Floater.IsNaN(args.NewValue)) {
				var newEnd = (Size)args.NewValue - 1;
				var newRange = new GameAnimationHelper.FrameRange() {
					Start = this.WorkingSpriteFrameRange.Start,
					Duration = newEnd - this.WorkingSpriteFrameRange.Start + 1,
				};
				if (newRange.Duration < 1) {
					newRange.Start = newEnd;
					newRange.Duration = 1;
				}
				if (newRange != this.WorkingSpriteFrameRange) {
					await this.UpdateWorkingSpriteFrameRange(newRange);
				}
			}
			this.NotifyPropertyChanged(
				nameof(this.uWorkingSpriteFrameRangeEnd_Value)
			);
			return;
		}

		#endregion

		#region working frame range label

		public Floater uWorkingSpriteFrameRangeLabelIcon_Opacity {
			get {
				if (!this.Working) {
					return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(false);
				}
				return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public String uWorkingSpriteFrameRangeLabel__ItemNameOfAll { get; } = "\0";

		public Boolean uWorkingSpriteFrameRangeLabel_IsEnabled {
			get {
				if (!this.Working) {
					return false;
				}
				return true;
			}
		}

		public List<String> uWorkingSpriteFrameRangeLabel_ItemsSource {
			get {
				if (!this.Working) {
					return [];
				}
				return [..this.WorkingSpriteFrameRangeLabelInformation.Select((value) => (value.Item1)), this.uWorkingSpriteFrameRangeLabel__ItemNameOfAll];
			}
		}

		public String? uWorkingSpriteFrameRangeLabel_SelectedItem {
			get {
				if (!this.Working) {
					return null;
				}
				var result = this.WorkingSpriteFrameRangeLabelInformation.Find((value) => (value.Item2 == this.WorkingSpriteFrameRange))?.Item1;
				return result ?? (this.WorkingSpriteFrameRange.Start == 0 && this.WorkingSpriteFrameRange.Duration == GameAnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.AsNotNull()).Frame.Count ? this.uWorkingSpriteFrameRangeLabel__ItemNameOfAll : null);
			}
		}

		public async void uWorkingSpriteFrameRangeLabel_SelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			var senders = sender.AsClass<ComboBox>();
			if (!this.Working) {
				return;
			}
			if (args.AddedItems.Count == 1) {
				var newLabel = args.AddedItems[0].AsClass<String>();
				var newRange = new GameAnimationHelper.FrameRange() {
					Start = 0,
					Duration = GameAnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.AsNotNull()).Frame.Count,
				};
				if (newLabel != this.uWorkingSpriteFrameRangeLabel__ItemNameOfAll) {
					newRange = this.WorkingSpriteFrameRangeLabelInformation.Find((value) => (value.Item1 == newLabel)).AsNotNull().Item2;
				}
				if (newRange != this.WorkingSpriteFrameRange) {
					await this.UpdateWorkingSpriteFrameRange(newRange);
				}
			}
			return;
		}

		#endregion

		#region working frame rate

		public Floater uWorkingSpriteFrameRateIcon_Opacity {
			get {
				if (!this.Working) {
					return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(false);
				}
				return ConvertHelper.MakeBooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public Boolean uWorkingSpriteFrameRate_IsEnabled {
			get {
				if (!this.Working) {
					return false;
				}
				return true;
			}
		}

		public DecimalFormatter uWorkingSpriteFrameRate_NumberFormatter {
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

		public Floater uWorkingSpriteFrameRate_Value {
			get {
				if (!this.Working) {
					return Floater.NaN;
				}
				return this.WorkingSpriteFrameRate.AsNotNull();
			}
		}

		public async void uWorkingSpriteFrameRate_ValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			var senders = sender.AsClass<NumberBox>();
			if (!this.Working) {
				return;
			}
			if (!Floater.IsNaN(args.NewValue)) {
				var newValue = args.NewValue;
				if (newValue != this.WorkingSpriteFrameRate) {
					this.View.uSprite.Speed = newValue;
					this.WorkingSpriteFrameRate = newValue;
				}
			}
			this.NotifyPropertyChanged(
				nameof(this.uWorkingSpriteFrameRate_Value)
			);
			return;
		}

		#endregion

		#region working frame progress

		public Boolean uWorkingSpriteFrameProgress_IsEnabled {
			get {
				if (!this.Working) {
					return false;
				}
				return true;
			}
		}

		public Floater uWorkingSpriteFrameProgress_Minimum {
			get {
				if (!this.Working) {
					return 0.0;
				}
				return this.WorkingSpriteFrameRange.Start + 1;
			}
		}

		public Floater uWorkingSpriteFrameProgress_Maximum {
			get {
				if (!this.Working) {
					return 0.0;
				}
				return this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration;
			}
		}

		public async void uWorkingSpriteFrameProgress_ValueChanged (
			Object                         sender,
			RangeBaseValueChangedEventArgs args
		) {
			var senders = sender.AsClass<Slider>();
			if (!this.Working) {
				return;
			}
			if (!Floater.IsNaN(args.NewValue) && args.NewValue != 0.0) {
				GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
				if (this.uWorkingSpriteFrameProgress__Changeable) {
					this.View.uSprite.CurrentTime = TimeSpan.FromSeconds(args.NewValue - 1.0);
				}
			}
			return;
		}

		// ----------------

		public Boolean uWorkingSpriteFrameProgress__Changeable = false;

		public Boolean uWorkingSpriteFrameProgress__ChangingWhenPlaying = false;

		public async void uWorkingSpriteFrameProgress_PointerPressed (
			Object                 sender,
			PointerRoutedEventArgs args
		) {
			var senders = sender.AsClass<Slider>();
			if (!this.Working) {
				return;
			}
			GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
			this.uWorkingSpriteFrameProgress__Changeable = true;
			this.uWorkingSpriteFrameProgress__ChangingWhenPlaying = this.WorkingSpriteState.AsNotNull();
			if (this.uWorkingSpriteFrameProgress__ChangingWhenPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
				this.WorkingSpriteState = false;
				this.NotifyPropertyChanged(
					nameof(this.uWorkingSpriteStateIcon_Glyph)
				);
			}
			return;
		}

		public async void uWorkingSpriteFrameProgress_PointerReleased (
			Object                 sender,
			PointerRoutedEventArgs args
		) {
			var senders = sender.AsClass<Slider>();
			if (!this.Working) {
				return;
			}
			GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
			if (this.uWorkingSpriteFrameProgress__ChangingWhenPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Playing;
				this.WorkingSpriteState = true;
				this.NotifyPropertyChanged(
					nameof(this.uWorkingSpriteStateIcon_Glyph)
				);
			}
			this.uWorkingSpriteFrameProgress__Changeable = false;
			this.uWorkingSpriteFrameProgress__ChangingWhenPlaying = false;
			return;
		}

		#endregion

		#region working frame progress control

		public Boolean uWorkingSpriteState_IsEnabled {
			get {
				if (!this.Working) {
					return false;
				}
				return true;
			}
		}

		public async void uWorkingSpriteState_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			if (!this.Working) {
				return;
			}
			GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
			var newState = this.View.uSprite.State != SpriteControl.StateType.Playing;
			this.View.uSprite.State = !newState ? SpriteControl.StateType.Paused : SpriteControl.StateType.Playing;
			this.WorkingSpriteState = newState;
			this.NotifyPropertyChanged(
				nameof(this.uWorkingSpriteStateIcon_Glyph)
			);
			return;
		}

		// ----------------

		public String uWorkingSpriteStateIcon_Glyph {
			get {
				if (!this.Working) {
					return FluentIconGlyph.Play;
				}
				return !this.WorkingSpriteState.AsNotNull() ? FluentIconGlyph.Play : FluentIconGlyph.Pause;
			}
		}

		// ----------------

		public Boolean uWorkingSpritePrevious_IsEnabled {
			get {
				if (!this.Working) {
					return false;
				}
				return true;
			}
		}

		public async void uWorkingSpritePrevious_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			if (!this.Working) {
				return;
			}
			GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
			var newTime = this.View.uSprite.CurrentTime - TimeSpan.FromSeconds(1.0);
			var beginTime = TimeSpan.FromSeconds(this.WorkingSpriteFrameRange.Start);
			if (newTime < beginTime) {
				newTime = beginTime;
			}
			this.View.uSprite.CurrentTime = newTime;
			return;
		}

		// ----------------

		public Boolean uWorkingSpriteNext_IsEnabled {
			get {
				if (!this.Working) {
					return false;
				}
				return true;
			}
		}

		public async void uWorkingSpriteNext_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<Button>();
			if (!this.Working) {
				return;
			}
			GF.AssertTest(this.View.uSprite.State != SpriteControl.StateType.Idle);
			var newTime = this.View.uSprite.CurrentTime + TimeSpan.FromSeconds(1.0);
			var endTime = TimeSpan.FromSeconds(this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration - 1.0);
			if (newTime > endTime) {
				newTime = endTime;
			}
			this.View.uSprite.CurrentTime = newTime;
			return;
		}

		#endregion

		#region show sprite boundary

		public Boolean uShowSpriteBoundary_IsChecked {
			get {
				return this.ShowSpriteBoundary;
			}
		}

		public async void uShowSpriteBoundary_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			this.ShowSpriteBoundary = senders.IsChecked.AsNotNull();
			this.View.uSprite.ShowBoundary = this.ShowSpriteBoundary;
			return;
		}

		#endregion

		#region filter rule

		public async void uFilterRule_Click (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			var senders = sender.AsClass<SplitButton>();
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
			var senders = sender.AsClass<TextBox>();
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
			var senders = sender.AsClass<TextBox>();
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
				var selectedItem = this.Animation.Sprite.Where((value, index) => (value.Name is not null && this.PlantCustomLayerName.Contains(value.Name) && this.SpriteFilter[index])).ToList();
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
			var senders = sender.AsClass<ComboBox>();
			if (!this.Loaded || this.PlantCustomLayerName.Count == 0) {
				return;
			}
			if (args.AddedItems.Count == 1) {
				var targetLayer = $"custom_{args.AddedItems[0].AsClass<String>()}";
				await this.ApplyFilter(null, this.Animation.Sprite.Select((value) => (value.Name is not null && this.PlantCustomLayerName.Contains(value.Name) ? value.Name == targetLayer : (Boolean?)null)).ToList());
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
				var selectedItem = this.Animation.Sprite.Where((value, index) => (value.Name is not null && this.ZombieStateLayerName.Contains(value.Name) && this.SpriteFilter[index])).ToList();
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
			var senders = sender.AsClass<ComboBox>();
			if (!this.Loaded || this.ZombieStateLayerName.Count == 0) { return; }
			if (args.AddedItems.Count == 1) {
				var targetLayer = args.AddedItems[0].AsClass<String>();
				await this.ApplyFilter(null, this.Animation.Sprite.Select((value) => (value.Name is not null && this.ZombieStateLayerName.Contains(value.Name) ? value.Name == targetLayer : (Boolean?)null)).ToList());
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
				var selectedItem = this.Animation.Sprite.Where((value, index) => (value.Name is not null && this.ZombieGroundSwatchLayerName.Contains(value.Name) && this.SpriteFilter[index])).ToList();
				return selectedItem.Count != 0;
			}
		}

		public async void uZombieGroundSwatchLayer_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			if (!this.Loaded || this.ZombieGroundSwatchLayerName.Count == 0) { return; }
			var newValue = senders.IsChecked.AsNotNull();
			await this.ApplyFilter(null, this.Animation.Sprite.Select((value) => (value.Name is not null && this.ZombieGroundSwatchLayerName.Contains(value.Name) ? newValue : (Boolean?)null)).ToList());
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
				var source = this.Host.ImageSource[this.Index];
				return $"{model.Size?[0] ?? source?.PixelWidth ?? 0} x {model.Size?[1] ?? source?.PixelHeight ?? 0}";
			}
		}

		public UIElement uPreview_Content {
			get {
				GF.AssertTest(this.Host.Loaded);
				var source = this.Host.ImageSource[this.Index];
				if (source is not null) {
					return new Image() {
						Width = 32, Height = 32,
						Source = source,
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
				var source = default(ImageSource?);
				if (model.Frame.Count == 1 && model.Frame[0].Append.Count == 1 && model.Frame[0].Change.Count == 1 && !model.Frame[0].Append[0].Sprite) {
					source = this.Host.ImageSource[(Size)model.Frame[0].Append[0].Resource];
				}
				if (source is not null) {
					return new Image() {
						Width = 32, Height = 32,
						Source = source,
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
				return this.Index == this.Host.WorkingSpriteIndex;
			}
		}

		public async void uToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			GF.AssertTest(this.Host.Loaded);
			var lastWorkingSpriteIndex = this.Host.WorkingSpriteIndex;
			var lastWorkingSpriteFrameRate = this.Host.WorkingSpriteFrameRate;
			if (this.Host.Working) {
				await this.Host.UnloadWorkingSprite();
			}
			if (this.Index != lastWorkingSpriteIndex) {
				await this.Host.LoadWorkingSprite(this.Index, null, !this.Host.RemainFrameRate ? null : lastWorkingSpriteFrameRate, null, null);
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
				if (this.Index is null) {
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
				if (this.Index is null) {
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
				if (this.Index is null) {
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
				if (this.Index is null) {
					return false;
				}
				else {
					GF.AssertTest(this.Host.Loaded);
					return this.Index == this.Host.WorkingSpriteIndex;
				}
			}
		}

		public async void uToggle_Click (
			Object          sender,
			RoutedEventArgs args
		) {
			var senders = sender.AsClass<ToggleButton>();
			if (this.Index is null) {
			}
			else {
				GF.AssertTest(this.Host.Loaded);
				var lastWorkingSpriteIndex = this.Host.WorkingSpriteIndex;
				var lastWorkingSpriteFrameRate = this.Host.WorkingSpriteFrameRate;
				if (this.Host.Working) {
					await this.Host.UnloadWorkingSprite();
				}
				if (this.Index != lastWorkingSpriteIndex) {
					await this.Host.LoadWorkingSprite(this.Index.AsNotNull(), null, !this.Host.RemainFrameRate ? null : lastWorkingSpriteFrameRate, null, null);
				}
			}
			return;
		}

		#endregion

	}

}
