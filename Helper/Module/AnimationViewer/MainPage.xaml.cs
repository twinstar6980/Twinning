#pragma warning disable 0,
// ReSharper disable

using Helper;
using Helper.Utility;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Media.Animation;
using Microsoft.UI.Xaml.Data;
using Microsoft.UI.Xaml.Input;
using Windows.Globalization.NumberFormatting;
using Windows.ApplicationModel.DataTransfer;
using Microsoft.UI.Xaml.Media.Imaging;
using Microsoft.UI.Xaml.Navigation;
using DecimalFormatter = Windows.Globalization.NumberFormatting.DecimalFormatter;
using IncrementNumberRounder = Windows.Globalization.NumberFormatting.IncrementNumberRounder;
using FluentIconGlyph = Helper.CustomControl.FluentIconGlyph;

namespace Helper.Module.AnimationViewer {

	public sealed partial class MainPage : Page {

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
			if (args.Parameter is List<String> option) {
				this.Controller.ApplyCommandOption(option);
			}
			base.OnNavigatedTo(args);
			return;
		}

		// ----------------

		private MainPageController Controller { get; }

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

		public String? AnimationFileSource { get; set; } = null;

		public String? ImageDirectorySource { get; set; } = null;

		// ----------------

		public AnimationModel.Animation? Animation { get; set; } = null;

		public List<BitmapSource?>? ImageSource { get; set; } = null;

		public List<Boolean>? ImageFilter { get; set; } = null;

		public List<Boolean>? SpriteFilter { get; set; } = null;

		// ----------------

		public Size? WorkingSpriteIndex { get; set; } = null;

		public AnimationHelper.FrameRange? WorkingSpriteFrameRange { get; set; } = null;

		public List<Tuple<String, AnimationHelper.FrameRange>>? WorkingSpriteFrameRangeLabelInformation { get; set; } = null;

		public Floater? WorkingSpriteFrameRate { get; set; } = null;

		public Boolean? WorkingSpriteState { get; set; } = null;

		// ----------------

		public Boolean ShowSpriteBoundary { get; set; } = false;

		public String SpriteFilterRule { get; set; } = "";

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
			this.ImmediateSelect = Setting.AnimationViewerImmediateSelect;
			this.AutomaticPlay = Setting.AnimationViewerAutomaticPlay;
			this.RepeatPlay = Setting.AnimationViewerRepeatPlay;
			this.RemainFrameRate = Setting.AnimationViewerRemainFrameRate;
			this.ShowSpriteBoundary = Setting.AnimationViewerShowSpriteBoundary;
			this.SpriteFilterRule = Setting.AnimationViewerSpriteFilterRule;
			this.View.uSprite.HoldEnd = true;
			this.View.uSprite.Repeat = true;
			this.View.uSprite.ShowBoundary = this.ShowSpriteBoundary;
			this.View.uWorkingSpriteFrameProgress.AddHandler(UIElement.PointerPressedEvent, new PointerEventHandler(this.uWorkingSpriteFrameProgress_OnPointerPressed), true);
			this.View.uWorkingSpriteFrameProgress.AddHandler(UIElement.PointerReleasedEvent, new PointerEventHandler(this.uWorkingSpriteFrameProgress_OnPointerReleased), true);
			return;
		}

		public async void ApplyCommandOption (
			List<String> optionView
		) {
			while (!this.View.IsLoaded) {
				await Task.Delay(TimeSpan.FromMilliseconds(50));
			}
			var optionImmediateSelect = default(Boolean?);
			var optionAutomaticPlay = default(Boolean?);
			var optionRepeatPlay = default(Boolean?);
			var optionRemainFrameRate = default(Boolean?);
			var optionShowSpriteBoundary = default(Boolean?);
			var optionSpriteFilterRule = default(String?);
			var optionSource = default(Tuple<String, String>?);
			var optionWorkingSpriteIndex = default(Integer?);
			var optionWorkingSpriteFrameRange = default(Tuple<Integer, Integer>?);
			var optionWorkingSpriteFrameRate = default(Floater?);
			var optionWorkingSpriteState = default(Boolean?);
			try {
				var option = new CommandLineReader(optionView);
				if (option.Ensure("-ImmediateSelect")) {
					optionImmediateSelect = option.NextBoolean();
				}
				if (option.Ensure("-AutomaticPlay")) {
					optionAutomaticPlay = option.NextBoolean();
				}
				if (option.Ensure("-RepeatPlay")) {
					optionRepeatPlay = option.NextBoolean();
				}
				if (option.Ensure("-RemainFrameRate")) {
					optionRemainFrameRate = option.NextBoolean();
				}
				if (option.Ensure("-ShowSpriteBoundary")) {
					optionShowSpriteBoundary = option.NextBoolean();
				}
				if (option.Ensure("-SpriteFilterRule")) {
					optionSpriteFilterRule = option.NextString();
				}
				if (option.Ensure("-Source")) {
					optionSource = new Tuple<String, String>(
						option.NextString(),
						option.NextString()
					);
				}
				if (option.Ensure("-WorkingSpriteIndex")) {
					optionWorkingSpriteIndex = option.NextInteger();
				}
				if (option.Ensure("-WorkingSpriteFrameRange")) {
					optionWorkingSpriteFrameRange = new Tuple<Integer, Integer>(
						option.NextInteger(),
						option.NextInteger()
					);
				}
				if (option.Ensure("-WorkingSpriteFrameRate")) {
					optionWorkingSpriteFrameRate = option.NextFloater();
				}
				if (option.Ensure("-WorkingSpriteState")) {
					optionWorkingSpriteState = option.NextBoolean();
				}
				if (!option.Done()) {
					throw new Exception($"Too many option : {String.Join(' ', option.NextStringList())}");
				}
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Failed to apply command option.", e.ToString());
			}
			if (optionImmediateSelect is not null) {
				this.ImmediateSelect = optionImmediateSelect.Value;
				this.NotifyPropertyChanged(
					nameof(this.uImmediateSelect_IsChecked)
				);
			}
			if (optionAutomaticPlay is not null) {
				this.AutomaticPlay = optionAutomaticPlay.Value;
				this.NotifyPropertyChanged(
					nameof(this.uAutomaticPlay_IsChecked)
				);
			}
			if (optionRepeatPlay is not null) {
				this.RepeatPlay = optionRepeatPlay.Value;
				this.NotifyPropertyChanged(
					nameof(this.uRepeatPlay_IsChecked)
				);
			}
			if (optionRemainFrameRate is not null) {
				this.RemainFrameRate = optionRemainFrameRate.Value;
				this.NotifyPropertyChanged(
					nameof(this.uRemainFrameRate_IsChecked)
				);
			}
			if (optionShowSpriteBoundary is not null) {
				this.ShowSpriteBoundary = optionShowSpriteBoundary.Value;
				this.NotifyPropertyChanged(
					nameof(this.uShowSpriteBoundary_IsChecked)
				);
			}
			if (optionSpriteFilterRule is not null) {
				this.SpriteFilterRule = optionSpriteFilterRule;
				this.NotifyPropertyChanged(
					nameof(this.uSpriteFilterRuleText_Text)
				);
			}
			if (optionSource is not null) {
				await this.ApplyLoad(
					optionSource.Item1,
					optionSource.Item2,
					optionWorkingSpriteIndex is null ? null : (Size)optionWorkingSpriteIndex,
					optionWorkingSpriteFrameRange is null ? null : new AnimationHelper.FrameRange() { Start = (Size)optionWorkingSpriteFrameRange.Item1, Duration = (Size)optionWorkingSpriteFrameRange.Item2 },
					optionWorkingSpriteFrameRate,
					optionWorkingSpriteState
				);
			}
			return;
		}

		#endregion

		#region load & work

		[MemberNotNullWhen(true, nameof(MainPageController.AnimationFileSource))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageDirectorySource))]
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

		[MemberNotNullWhen(true, nameof(MainPageController.AnimationFileSource))]
		[MemberNotNullWhen(true, nameof(MainPageController.ImageDirectorySource))]
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
			String animationFileSource,
			String imageDirectorySource
		) {
			Debug.Assert(!this.Loaded && !this.Working);
			var animationData = default(AnimationModel.Animation?);
			var imageSourceData = default(List<BitmapSource?>?);
			try {
				animationData = await AnimationHelper.LoadAnimation(animationFileSource);
				imageSourceData = await AnimationHelper.LoadImageSource(imageDirectorySource, animationData);
			} catch (Exception e) {
				MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Failed to load animation.", e.ToString());
				return;
			}
			this.AnimationFileSource = animationFileSource;
			this.ImageDirectorySource = imageDirectorySource;
			this.Animation = animationData;
			this.ImageSource = imageSourceData;
			this.ImageFilter = Enumerable.Range(0, this.Animation.Image.Count).Select((index) => (false)).ToList();
			this.SpriteFilter = Enumerable.Range(0, this.Animation.Sprite.Count).Select((index) => (false)).ToList();
			this.PlantCustomLayerName = this.Animation.Sprite.Select((value) => (value.Name)).Where((value) => (value.StartsWith("custom_"))).ToList();
			this.ZombieStateLayerName = this.Animation.Sprite.Select((value) => (value.Name)).Where((value) => (value == "ink" || value == "butter")).ToList();
			this.ZombieGroundSwatchLayerName = this.Animation.Sprite.Select((value) => (value.Name)).Where((value) => (value == "ground_swatch" || value == "ground_swatch_plane")).ToList();
			this.NotifyPropertyChanged(
				nameof(this.uClearSource_IsEnabled),
				nameof(this.uAnimationFileSource_IsEnabled),
				nameof(this.uAnimationFileSource_Text),
				nameof(this.uImageDirectorySource_IsEnabled),
				nameof(this.uImageDirectorySource_Text),
				nameof(this.uSelectImageDirectorySource_IsEnabled),
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
			Debug.Assert(this.Loaded && !this.Working);
			this.View.uImageList.DeselectRange(new ItemIndexRange(0, (USize)this.Animation.Image.Count));
			this.View.uSpriteList.DeselectRange(new ItemIndexRange(0, (USize)this.Animation.Sprite.Count));
			if (this.Animation.MainSprite is not null) {
				this.View.uMainSpriteList.DeselectRange(new ItemIndexRange(0, 1));
			}
			this.AnimationFileSource = null;
			this.ImageDirectorySource = null;
			this.Animation = null;
			this.ImageSource = null;
			this.ImageFilter = null;
			this.SpriteFilter = null;
			this.PlantCustomLayerName = null;
			this.ZombieStateLayerName = null;
			this.ZombieGroundSwatchLayerName = null;
			this.NotifyPropertyChanged(
				nameof(this.uClearSource_IsEnabled),
				nameof(this.uAnimationFileSource_IsEnabled),
				nameof(this.uAnimationFileSource_Text),
				nameof(this.uImageDirectorySource_IsEnabled),
				nameof(this.uImageDirectorySource_Text),
				nameof(this.uSelectImageDirectorySource_IsEnabled),
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
			Size                        index,
			AnimationHelper.FrameRange? frameRange,
			Floater?                    frameRate,
			Boolean?                    initialState,
			TimeSpan?                   initialTime
		) {
			Debug.Assert(this.Loaded && !this.Working);
			var workingSprite = AnimationHelper.SelectSprite(this.Animation, index);
			this.WorkingSpriteIndex = index;
			this.WorkingSpriteFrameRange = frameRange ?? new AnimationHelper.FrameRange() { Start = 0, Duration = workingSprite.Frame.Count };
			this.WorkingSpriteFrameRangeLabelInformation = new List<Tuple<String, AnimationHelper.FrameRange>>();
			var currentFrameLabel = new List<Tuple<String, Size>>();
			for (var frameIndex = 0; frameIndex < workingSprite.Frame.Count; ++frameIndex) {
				var frame = workingSprite.Frame[frameIndex];
				if (frame.Label is not null) {
					currentFrameLabel.Add(new Tuple<String, Size>(frame.Label, frameIndex));
				}
				if (frame.Stop) {
					foreach (var item in currentFrameLabel) {
						this.WorkingSpriteFrameRangeLabelInformation.Add(new Tuple<String, AnimationHelper.FrameRange>(item.Item1, new AnimationHelper.FrameRange() {
							Start = item.Item2,
							Duration = frameIndex - item.Item2 + 1,
						}));
					}
					currentFrameLabel.Clear();
				}
			}
			this.WorkingSpriteFrameRate = frameRate ?? workingSprite.FrameRate;
			this.WorkingSpriteState = initialState ?? this.AutomaticPlay;
			this.View.uSprite.Load(this.Animation, this.ImageSource, this.ImageFilter, this.SpriteFilter, index);
			Debug.Assert(this.View.uSprite.Loaded);
			var sliderAnimation = new ObjectAnimationUsingKeyFrames();
			for (var frameIndex = 0; frameIndex < workingSprite.Frame.Count; ++frameIndex) {
				sliderAnimation.KeyFrames.Add(
					new DiscreteObjectKeyFrame() {
						KeyTime = KeyTime.FromTimeSpan(TimeSpan.FromSeconds(frameIndex)),
						Value = (Floater)(frameIndex + 1),
					}
				);
			}
			Storyboard.SetTargetProperty(sliderAnimation, "Value");
			var sliderStoryboard = new Storyboard() {
				Duration = new Duration(TimeSpan.FromSeconds(workingSprite.Frame.Count)),
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
			this.View.uSprite.Speed = this.WorkingSpriteFrameRate.Value;
			this.View.uSprite.State = !this.WorkingSpriteState.Value ? SpriteControl.StateType.Paused : SpriteControl.StateType.Playing;
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
			foreach (var item in (this.View.uSpriteList.ItemsSource as List<SpriteItemController>)!) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			foreach (var item in (this.View.uMainSpriteList.ItemsSource as List<MainSpriteItemController>)!) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			return;
		}

		public async Task UnloadWorkingSprite (
		) {
			Debug.Assert(this.Loaded && this.Working);
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
			foreach (var item in (this.View.uSpriteList.ItemsSource as List<SpriteItemController>)!) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			foreach (var item in (this.View.uMainSpriteList.ItemsSource as List<MainSpriteItemController>)!) {
				item.NotifyPropertyChanged(nameof(item.uToggle_IsChecked));
			}
			this.View.uSprite.Unload();
			return;
		}

		public async Task UpdateWorkingSpriteFrameRange (
			AnimationHelper.FrameRange frameRange
		) {
			Debug.Assert(this.Loaded && this.Working);
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
			String                      animationFileSource,
			String                      imageDirectorySource,
			Size?                       spriteIndex,
			AnimationHelper.FrameRange? spriteFrameRange,
			Floater?                    spriteFrameRate,
			Boolean?                    spriteInitialState
		) {
			if (this.Loaded) {
				if (this.Working) {
					await this.UnloadWorkingSprite();
				}
				await this.Unload();
			}
			await this.Load(animationFileSource, imageDirectorySource);
			if (this.Loaded) {
				await this.ApplySpriteFilterRule();
				if (this.ImmediateSelect && this.Animation.MainSprite is not null) {
					await this.LoadWorkingSprite(spriteIndex ?? this.Animation.Sprite.Count, spriteFrameRange, spriteFrameRate, spriteInitialState, null);
				}
				App.AppendRecentJumpList($"Animation Viewer - {Regex.Replace(StorageHelper.GetPathName(animationFileSource), @"(\.pam\.json)$", "", RegexOptions.IgnoreCase)}", new List<String>() {
					"-ModuleType",
					ModuleType.AnimationViewer.ToString(),
					"-ModuleOption",
					"-Source",
					animationFileSource,
					imageDirectorySource,
				});
			}
			return;
		}

		public async Task ApplyFilter (
			List<Boolean?>? imageFilter,
			List<Boolean?>? spriteFilter
		) {
			Debug.Assert(this.Loaded);
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
				for (var index = 0; index < imageFilter.Count; ++index) {
					var state = imageFilter[index];
					if (state is null || state.Value == this.ImageFilter[index]) {
						continue;
					}
					this.ImageFilter[index] = state.Value;
					if (state.Value) {
						this.View.uImageList.SelectRange(new ItemIndexRange(index, 1));
					} else {
						this.View.uImageList.DeselectRange(new ItemIndexRange(index, 1));
					}
				}
			}
			if (spriteFilter is not null) {
				for (var index = 0; index < spriteFilter.Count; ++index) {
					var state = spriteFilter[index];
					if (state is null || state.Value == this.SpriteFilter[index]) {
						continue;
					}
					this.SpriteFilter[index] = state.Value;
					if (state.Value) {
						this.View.uSpriteList.SelectRange(new ItemIndexRange(index, 1));
					} else {
						this.View.uSpriteList.DeselectRange(new ItemIndexRange(index, 1));
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
				await this.LoadWorkingSprite(spriteIndex.Value, frameRange, frameRate, currentState, currentTime);
			}
			this.SuppressApplyFilterChanged = false;
			return;
		}

		public async Task ApplySpriteFilterRule (
		) {
			Debug.Assert(this.Loaded);
			if (this.SpriteFilterRule.Length != 0) {
				await this.ApplyFilter(null, this.Animation.Sprite.Select((value) => ((Boolean?)!Regex.IsMatch(value.Name, this.SpriteFilterRule))).ToList());
			}
			return;
		}

		#endregion

		#region page

		public async void uPage_OnDragOver (
			Object        sender,
			DragEventArgs args
		) {
			if (sender is not Page senders) { return; }
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				args.AcceptedOperation = Windows.ApplicationModel.DataTransfer.DataPackageOperation.Link;
			}
			return;
		}

		public async void uPage_OnDrop (
			Object        sender,
			DragEventArgs args
		) {
			if (sender is not Page senders) { return; }
			if (args.DataView.Contains(StandardDataFormats.StorageItems)) {
				var item = await args.DataView.GetStorageItemsAsync();
				if (item.Count != 1) {
					MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Source is multiply.", "");
					return;
				}
				var animationFile = StorageHelper.Normalize(item[0].Path);
				if (!StorageHelper.ExistFile(animationFile)) {
					MainWindow.Instance.Controller.PublishTip(InfoBarSeverity.Error, "Source is not a file.", "");
					return;
				}
				var imageDirectory = StorageHelper.GetPathParent(animationFile) ?? throw new Exception();
				await this.ApplyLoad(animationFile, imageDirectory, null, null, null, null);
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

		// ----------------

		public String uStagePositionY_Text {
			get {
				return $"{this.StagePositionY:F0}";
			}
		}

		// ----------------

		public String uStageScale_Text {
			get {
				return $"{this.StageScale * 100:F0} %";
			}
		}

		// ----------------

		public String uStageScaleIcon_Glyph {
			get {
				var valueLose = (Size)Math.Round(this.StageScale * 100.0);
				if (valueLose < 100) {
					return FluentIconGlyph.BackToWindow;
				} else if (valueLose > 100) {
					return FluentIconGlyph.FullScreen;
				} else {
					return FluentIconGlyph.Reshare;
				}
			}
		}

		// ----------------

		public async void uStage_OnViewChanged (
			Object sender,
			Object args
		) {
			if (sender is not ScrollViewer senders) { return; }
			var newScale = senders.ZoomFactor;
			var newPositionX = (senders.HorizontalOffset + senders.ActualWidth / 2) / senders.ZoomFactor;
			var newPositionY = (senders.VerticalOffset + senders.ActualHeight / 2) / senders.ZoomFactor;
			this.StageScale = newScale;
			this.StagePositionX = newPositionX;
			this.StagePositionY = newPositionY;
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

		public async void uImmediateSelect_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			this.ImmediateSelect = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uImmediateSelect_IsChecked)
			);
			return;
		}

		// ----------------

		public Boolean uAutomaticPlay_IsChecked {
			get {
				return this.AutomaticPlay;
			}
		}

		public async void uAutomaticPlay_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			this.AutomaticPlay = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uAutomaticPlay_IsChecked)
			);
			return;
		}

		// ----------------

		public Boolean uRepeatPlay_IsChecked {
			get {
				return this.RepeatPlay;
			}
		}

		public async void uRepeatPlay_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			this.RepeatPlay = newValue;
			this.View.uSprite.Repeat = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uRepeatPlay_IsChecked)
			);
			return;
		}

		// ----------------

		public Boolean uRemainFrameRate_IsChecked {
			get {
				return this.RemainFrameRate;
			}
		}

		public async void uRemainFrameRate_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			this.RemainFrameRate = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uRemainFrameRate_IsChecked)
			);
			return;
		}

		#endregion

		#region source

		public Boolean uClearSource_IsEnabled {
			get {
				if (!this.Loaded) { return false; }
				return true;
			}
		}

		public async void uClearSource_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			if (!this.Loaded) { return; }
			if (this.Working) {
				await this.UnloadWorkingSprite();
			}
			await this.Unload();
			return;
		}

		// ----------------

		public Boolean uAnimationFileSource_IsEnabled {
			get {
				if (!this.Loaded) { return false; }
				return true;
			}
		}

		public String uAnimationFileSource_Text {
			get {
				if (!this.Loaded) { return ""; }
				return this.AnimationFileSource;
			}
		}

		// ----------------

		public async void uSelectAnimationFileSource_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			var isPlaying = this.Working && this.View.uSprite.State == SpriteControl.StateType.Playing;
			if (isPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
			}
			var animationFile = await StorageHelper.PickFile(WindowHelper.GetForElement(this.View), ".json");
			if (animationFile is not null) {
				var imageDirectory = StorageHelper.GetPathParent(animationFile) ?? throw new Exception();
				await this.ApplyLoad(animationFile, imageDirectory, null, null, null, null);
			} else {
				if (isPlaying) {
					this.View.uSprite.State = SpriteControl.StateType.Playing;
				}
			}
			return;
		}

		// ----------------

		public Boolean uImageDirectorySource_IsEnabled {
			get {
				if (!this.Loaded) { return false; }
				return true;
			}
		}

		public String uImageDirectorySource_Text {
			get {
				if (!this.Loaded) { return ""; }
				return this.ImageDirectorySource;
			}
		}

		// ----------------

		public Boolean uSelectImageDirectorySource_IsEnabled {
			get {
				if (!this.Loaded) { return false; }
				return true;
			}
		}

		public async void uSelectImageDirectorySource_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			if (!this.Loaded) { return; }
			var isPlaying = this.Working && this.View.uSprite.State == SpriteControl.StateType.Playing;
			if (isPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
			}
			var imageDirectory = await StorageHelper.PickDirectory(WindowHelper.GetForElement(this.View));
			if (imageDirectory is not null) {
				var animationFile = this.AnimationFileSource;
				await this.ApplyLoad(animationFile, imageDirectory, null, null, null, null);
			} else {
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
				if (!this.Loaded) { return false; }
				return true;
			}
		}

		public List<ImageItemController> uImageList_ItemsSource {
			get {
				if (!this.Loaded) { return new List<ImageItemController>(); }
				return new List<ImageItemController>(Enumerable.Range(0, this.Animation.Image.Count).Select((index) => (new ImageItemController() { Host = this, Index = index })));
			}
		}

		public async void uImageList_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ListView senders) { return; }
			if (!this.Loaded) { return; }
			if (this.SuppressFilterListSelectionChanged) { return; }
			foreach (var item in args.AddedItems) {
				var itemX = item as ImageItemController ?? throw new NullReferenceException();
				this.ImageFilter[itemX.Index] = true;
			}
			foreach (var item in args.RemovedItems) {
				var itemX = item as ImageItemController ?? throw new NullReferenceException();
				this.ImageFilter[itemX.Index] = false;
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
				await this.LoadWorkingSprite(spriteIndex.Value, frameRange, frameRate, currentState, currentTime);
			}
			return;
		}

		// ----------------

		public Boolean uSpriteList_IsEnabled {
			get {
				if (!this.Loaded) { return false; }
				return true;
			}
		}

		public List<SpriteItemController> uSpriteList_ItemsSource {
			get {
				if (!this.Loaded) { return new List<SpriteItemController>(); }
				return new List<SpriteItemController>(Enumerable.Range(0, this.Animation.Sprite.Count).Select((index) => (new SpriteItemController() { Host = this, Index = index })));
			}
		}

		public async void uSpriteList_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ListView senders) { return; }
			if (!this.Loaded) { return; }
			if (this.SuppressFilterListSelectionChanged) { return; }
			foreach (var item in args.AddedItems) {
				var itemX = item as SpriteItemController ?? throw new NullReferenceException();
				this.SpriteFilter[itemX.Index] = true;
			}
			foreach (var item in args.RemovedItems) {
				var itemX = item as SpriteItemController ?? throw new NullReferenceException();
				this.SpriteFilter[itemX.Index] = false;
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
				await this.LoadWorkingSprite(spriteIndex.Value, frameRange, frameRate, currentState, currentTime);
			}
			return;
		}

		// ----------------

		public Boolean uMainSpriteList_IsEnabled {
			get {
				if (!this.Loaded) { return false; }
				return this.Animation.MainSprite is not null;
			}
		}

		public List<MainSpriteItemController> uMainSpriteList_ItemsSource {
			get {
				if (!this.Loaded) { return new List<MainSpriteItemController>() { new MainSpriteItemController { Host = this, Index = null } }; }
				return new List<MainSpriteItemController>() { new MainSpriteItemController { Host = this, Index = this.Animation.MainSprite is null ? null : this.Animation.Sprite.Count } };
			}
		}

		public async void uMainSpriteList_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ListView senders) { return; }
			if (!this.Loaded) { return; }
			return;
		}

		#endregion

		#region working frame range

		public Floater uWorkingSpriteFrameRangeIcon_Opacity {
			get {
				if (!this.Working) { return ConvertHelper.BooleanToFloaterOfOpacityEnabled(false); }
				return ConvertHelper.BooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public Boolean uWorkingSpriteFrameRangeBegin_IsEnabled {
			get {
				if (!this.Working) { return false; }
				return true;
			}
		}

		public DecimalFormatter uWorkingSpriteFrameRangeBegin_NumberFormatter {
			get {
				return new DecimalFormatter() { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uWorkingSpriteFrameRangeBegin_Minimum {
			get {
				if (!this.Working) { return Floater.NaN; }
				return 1.0;
			}
		}

		public Floater uWorkingSpriteFrameRangeBegin_Maximum {
			get {
				if (!this.Working) { return Floater.NaN; }
				return AnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.Value).Frame.Count;
			}
		}

		public Floater uWorkingSpriteFrameRangeBegin_Value {
			get {
				if (!this.Working) { return Floater.NaN; }
				return this.WorkingSpriteFrameRange.Start + 1;
			}
		}

		public async void uWorkingSpriteFrameRangeBegin_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			if (sender is not NumberBox senders) { return; }
			if (!this.Working) { return; }
			if (!Floater.IsNaN(args.NewValue)) {
				var newBegin = (Size)args.NewValue - 1;
				var newRange = new AnimationHelper.FrameRange() {
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
				if (!this.Working) { return false; }
				return true;
			}
		}

		public DecimalFormatter uWorkingSpriteFrameRangeEnd_NumberFormatter {
			get {
				return new DecimalFormatter() { IntegerDigits = 1, FractionDigits = 0 };
			}
		}

		public Floater uWorkingSpriteFrameRangeEnd_Minimum {
			get {
				if (!this.Working) { return Floater.NaN; }
				return 1.0;
			}
		}

		public Floater uWorkingSpriteFrameRangeEnd_Maximum {
			get {
				if (!this.Working) { return Floater.NaN; }
				return AnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.Value).Frame.Count;
			}
		}

		public Floater uWorkingSpriteFrameRangeEnd_Value {
			get {
				if (!this.Working) { return Floater.NaN; }
				return this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration;
			}
		}

		public async void uWorkingSpriteFrameRangeEnd_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			if (sender is not NumberBox senders) { return; }
			if (!this.Working) { return; }
			if (!Floater.IsNaN(args.NewValue)) {
				var newEnd = (Size)args.NewValue - 1;
				var newRange = new AnimationHelper.FrameRange() {
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
				if (!this.Working) { return ConvertHelper.BooleanToFloaterOfOpacityEnabled(false); }
				return ConvertHelper.BooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public String uWorkingSpriteFrameRangeLabel__ItemNameOfAll { get; } = "\0";

		public Boolean uWorkingSpriteFrameRangeLabel_IsEnabled {
			get {
				if (!this.Working) { return false; }
				return true;
			}
		}

		public List<String> uWorkingSpriteFrameRangeLabel_ItemsSource {
			get {
				if (!this.Working) { return new List<String>(); }
				var result = this.WorkingSpriteFrameRangeLabelInformation.Select((value) => (value.Item1)).ToList();
				result.Add(this.uWorkingSpriteFrameRangeLabel__ItemNameOfAll);
				return result;
			}
		}

		public String? uWorkingSpriteFrameRangeLabel_SelectedItem {
			get {
				if (!this.Working) { return null; }
				var result = this.WorkingSpriteFrameRangeLabelInformation.Find((value) => (value.Item2 == this.WorkingSpriteFrameRange))?.Item1;
				return result ?? (this.WorkingSpriteFrameRange.Start == 0 && this.WorkingSpriteFrameRange.Duration == AnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.Value).Frame.Count ? this.uWorkingSpriteFrameRangeLabel__ItemNameOfAll : null);
			}
		}

		public async void uWorkingSpriteFrameRangeLabel_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ComboBox senders) { return; }
			if (!this.Working) { return; }
			if (args.AddedItems.Count == 1) {
				var newLabel = args.AddedItems[0] as String ?? throw new NullReferenceException();
				var newRange = new AnimationHelper.FrameRange() {
					Start = 0,
					Duration = AnimationHelper.SelectSprite(this.Animation, this.WorkingSpriteIndex.Value).Frame.Count,
				};
				if (newLabel != this.uWorkingSpriteFrameRangeLabel__ItemNameOfAll) {
					newRange = this.WorkingSpriteFrameRangeLabelInformation.Find((value) => (value.Item1 == newLabel))!.Item2;
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
				if (!this.Working) { return ConvertHelper.BooleanToFloaterOfOpacityEnabled(false); }
				return ConvertHelper.BooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public Boolean uWorkingSpriteFrameRate_IsEnabled {
			get {
				if (!this.Working) { return false; }
				return true;
			}
		}

		public DecimalFormatter uWorkingSpriteFrameRate_NumberFormatter {
			get {
				return new DecimalFormatter() {
					IntegerDigits = 1,
					FractionDigits = 1,
					NumberRounder = new IncrementNumberRounder {
						Increment = 0.1,
						RoundingAlgorithm = RoundingAlgorithm.RoundHalfUp,
					},
				};
			}
		}

		public Floater uWorkingSpriteFrameRate_Value {
			get {
				if (!this.Working) { return Floater.NaN; }
				return this.WorkingSpriteFrameRate.Value;
			}
		}

		public async void uWorkingSpriteFrameRate_OnValueChanged (
			NumberBox                      sender,
			NumberBoxValueChangedEventArgs args
		) {
			if (sender is not NumberBox senders) { return; }
			if (!this.Working) { return; }
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
				if (!this.Working) { return false; }
				return true;
			}
		}

		public Floater uWorkingSpriteFrameProgress_Minimum {
			get {
				if (!this.Working) { return 0.0; }
				return this.WorkingSpriteFrameRange.Start + 1;
			}
		}

		public Floater uWorkingSpriteFrameProgress_Maximum {
			get {
				if (!this.Working) { return 0.0; }
				return this.WorkingSpriteFrameRange.Start + this.WorkingSpriteFrameRange.Duration;
			}
		}

		public async void uWorkingSpriteFrameProgress_OnValueChanged (
			Object                         sender,
			RangeBaseValueChangedEventArgs args
		) {
			if (sender is not Slider senders) { return; }
			if (!this.Working) { return; }
			if (!Floater.IsNaN(args.NewValue) && args.NewValue != 0.0) {
				Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
				if (this.uWorkingSpriteFrameProgress__Changeable) {
					this.View.uSprite.CurrentTime = TimeSpan.FromSeconds(args.NewValue - 1.0);
				}
			}
			return;
		}

		// ----------------

		public Boolean uWorkingSpriteFrameProgress__Changeable = false;

		public Boolean uWorkingSpriteFrameProgress__ChangingWhenPlaying = false;

		public async void uWorkingSpriteFrameProgress_OnPointerPressed (
			Object                 sender,
			PointerRoutedEventArgs args
		) {
			if (sender is not Slider senders) { return; }
			if (!this.Working) { return; }
			Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
			this.uWorkingSpriteFrameProgress__Changeable = true;
			this.uWorkingSpriteFrameProgress__ChangingWhenPlaying = this.WorkingSpriteState.Value;
			if (this.uWorkingSpriteFrameProgress__ChangingWhenPlaying) {
				this.View.uSprite.State = SpriteControl.StateType.Paused;
				this.WorkingSpriteState = false;
				this.NotifyPropertyChanged(
					nameof(this.uWorkingSpriteStateIcon_Glyph)
				);
			}
			return;
		}

		public async void uWorkingSpriteFrameProgress_OnPointerReleased (
			Object                 sender,
			PointerRoutedEventArgs args
		) {
			if (sender is not Slider senders) { return; }
			if (!this.Working) { return; }
			Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
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
				if (!this.Working) { return false; }
				return true;
			}
		}

		public async void uWorkingSpriteState_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			if (!this.Working) { return; }
			Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
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
				if (!this.Working) { return FluentIconGlyph.Play; }
				return !this.WorkingSpriteState.Value ? FluentIconGlyph.Play : FluentIconGlyph.Pause;
			}
		}

		// ----------------

		public Boolean uWorkingSpritePrevious_IsEnabled {
			get {
				if (!this.Working) { return false; }
				return true;
			}
		}

		public async void uWorkingSpritePrevious_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			if (!this.Working) { return; }
			Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
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
				if (!this.Working) { return false; }
				return true;
			}
		}

		public async void uWorkingSpriteNext_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not Button senders) { return; }
			if (!this.Working) { return; }
			Debug.Assert(this.View.uSprite.State != SpriteControl.StateType.Idle);
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

		public async void uShowSpriteBoundary_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			var newValue = senders.IsChecked!.Value;
			this.ShowSpriteBoundary = newValue;
			this.View.uSprite.ShowBoundary = this.ShowSpriteBoundary;
			this.NotifyPropertyChanged(
				nameof(this.uShowSpriteBoundary_IsChecked)
			);
			return;
		}

		#endregion

		#region sprite filter rule

		public async void uSpriteFilterRule_OnClick (
			SplitButton               sender,
			SplitButtonClickEventArgs args
		) {
			if (sender is not SplitButton senders) { return; }
			if (!this.Loaded) { return; }
			await this.ApplySpriteFilterRule();
			return;
		}

		// ----------------

		public String uSpriteFilterRuleText_Text {
			get {
				return this.SpriteFilterRule;
			}
		}

		public async void uSpriteFilterRuleText_OnTextChanged (
			Object               sender,
			TextChangedEventArgs args
		) {
			if (sender is not TextBox senders) { return; }
			var newValue = senders.Text;
			this.SpriteFilterRule = newValue;
			this.NotifyPropertyChanged(
				nameof(this.uSpriteFilterRuleText_Text)
			);
			return;
		}

		#endregion

		#region plant custom layer

		public Floater uPlantCustomLayerIcon_Opacity {
			get {
				if (!this.Loaded || this.PlantCustomLayerName.Count == 0) {
					return ConvertHelper.BooleanToFloaterOfOpacityEnabled(false);
				}
				return ConvertHelper.BooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public String uPlantCustomLayer__ItemNameOfNone { get; } = "\0";

		public Boolean uPlantCustomLayer_IsEnabled {
			get {
				if (!this.Loaded || this.PlantCustomLayerName.Count == 0) { return false; }
				return true;
			}
		}

		public List<String> uPlantCustomLayer_ItemsSource {
			get {
				if (!this.Loaded || this.PlantCustomLayerName.Count == 0) { return new List<String>(); }
				var result = this.PlantCustomLayerName.Select((value) => (value["custom_".Length..])).ToList();
				result.Add(this.uPlantCustomLayer__ItemNameOfNone);
				return result;
			}
		}

		public String? uPlantCustomLayer_SelectedItem {
			get {
				if (!this.Loaded || this.PlantCustomLayerName.Count == 0) { return null; }
				var selectedItem = this.Animation.Sprite.Select((value) => (value.Name)).Where((value, index) => (this.PlantCustomLayerName.Contains(value) && this.SpriteFilter[index])).ToList();
				return selectedItem.Count switch {
					0 => this.uPlantCustomLayer__ItemNameOfNone,
					1 => selectedItem[0]["custom_".Length..],
					_ => null,
				};
			}
		}

		public async void uPlantCustomLayer_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ComboBox senders) { return; }
			if (!this.Loaded || this.PlantCustomLayerName.Count == 0) { return; }
			if (args.AddedItems.Count == 1) {
				var targetLayer = $"custom_{args.AddedItems[0] as String ?? throw new NullReferenceException()}";
				await this.ApplyFilter(null, this.Animation.Sprite.Select((value) => (!this.PlantCustomLayerName.Contains(value.Name) ? (Boolean?)null : value.Name == targetLayer)).ToList());
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
				if (!this.Loaded || this.ZombieStateLayerName.Count == 0) { return ConvertHelper.BooleanToFloaterOfOpacityEnabled(false); }
				return ConvertHelper.BooleanToFloaterOfOpacityEnabled(true);
			}
		}

		// ----------------

		public String uZombieStateLayer__ItemNameOfNone { get; } = "\0";

		public Boolean uZombieStateLayer_IsEnabled {
			get {
				if (!this.Loaded || this.ZombieStateLayerName.Count == 0) { return false; }
				return true;
			}
		}

		public List<String> uZombieStateLayer_ItemsSource {
			get {
				if (!this.Loaded || this.ZombieStateLayerName.Count == 0) { return new List<String>(); }
				var result = new List<String>(this.ZombieStateLayerName);
				result.Add(this.uZombieStateLayer__ItemNameOfNone);
				return result;
			}
		}

		public String? uZombieStateLayer_SelectedItem {
			get {
				if (!this.Loaded || this.ZombieStateLayerName.Count == 0) { return null; }
				var selectedItem = this.Animation.Sprite.Select((value) => (value.Name)).Where((value, index) => (this.ZombieStateLayerName.Contains(value) && this.SpriteFilter[index])).ToList();
				return selectedItem.Count switch {
					0 => this.uZombieStateLayer__ItemNameOfNone,
					1 => selectedItem[0],
					_ => null,
				};
			}
		}

		public async void uZombieStateLayer_OnSelectionChanged (
			Object                    sender,
			SelectionChangedEventArgs args
		) {
			if (sender is not ComboBox senders) { return; }
			if (!this.Loaded || this.ZombieStateLayerName.Count == 0) { return; }
			if (args.AddedItems.Count == 1) {
				var targetLayer = args.AddedItems[0] as String ?? throw new NullReferenceException();
				await this.ApplyFilter(null, this.Animation.Sprite.Select((value) => (!this.ZombieStateLayerName.Contains(value.Name) ? (Boolean?)null : value.Name == targetLayer)).ToList());
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
				if (!this.Loaded || this.ZombieGroundSwatchLayerName.Count == 0) { return ConvertHelper.BooleanToFloaterOfOpacityEnabled(false); }
				return ConvertHelper.BooleanToFloaterOfOpacityEnabled(true);
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
				var selectedItem = this.Animation.Sprite.Select((value) => (value.Name)).Where((value, index) => (this.ZombieGroundSwatchLayerName.Contains(value) && this.SpriteFilter[index])).ToList();
				return selectedItem.Count != 0;
			}
		}

		public async void uZombieGroundSwatchLayer_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			if (!this.Loaded || this.ZombieGroundSwatchLayerName.Count == 0) { return; }
			var newValue = senders.IsChecked!.Value;
			await this.ApplyFilter(null, this.Animation.Sprite.Select((value) => (!this.ZombieGroundSwatchLayerName.Contains(value.Name) ? (Boolean?)null : newValue)).ToList());
			this.NotifyPropertyChanged(
				nameof(this.uZombieGroundSwatchLayer_IsChecked)
			);
			return;
		}

		#endregion

	}

	public class ImageItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public Size Index { get; set; } = default!;

		#endregion

		#region view

		public String uTitle_Text {
			get {
				Debug.Assert(this.Host.Loaded);
				var model = this.Host.Animation.Image[this.Index];
				return $"{model.Name.Split('|')[0]}";
			}
		}

		// ----------------

		public String uDescription_Text {
			get {
				Debug.Assert(this.Host.Loaded);
				var model = this.Host.Animation.Image[this.Index];
				return $"{model.Size[0]} x {model.Size[1]}";
			}
		}

		// ----------------

		public UIElement uPreview_Content {
			get {
				Debug.Assert(this.Host.Loaded);
				var source = this.Host.ImageSource[this.Index];
				if (source is not null) {
					return new Image() {
						Width = 32, Height = 32,
						Source = source,
					};
				} else {
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

	public class SpriteItemController : CustomController {

		#region data

		public MainPageController Host { get; init; } = default!;

		// ----------------

		public Size Index { get; set; } = default!;

		#endregion

		#region view

		public String uTitle_Text {
			get {
				Debug.Assert(this.Host.Loaded);
				var model = this.Host.Animation.Sprite[this.Index];
				return $"{model.Name}";
			}
		}

		// ----------------

		public String uDescription_Text {
			get {
				Debug.Assert(this.Host.Loaded);
				var model = this.Host.Animation.Sprite[this.Index];
				return $"{model.FrameRate:F0} - {model.Frame.Count}";
			}
		}

		// ----------------

		public UIElement uPreview_Content {
			get {
				Debug.Assert(this.Host.Loaded);
				var model = this.Host.Animation.Sprite[this.Index];
				var source = (ImageSource?)null;
				if (model.Frame.Count == 1 && model.Frame[0].Append.Count == 1 && model.Frame[0].Change.Count == 1 && !model.Frame[0].Append[0].Sprite) {
					source = this.Host.ImageSource[(Size)model.Frame[0].Append[0].Resource];
				}
				if (source is not null) {
					return new Image() {
						Width = 32, Height = 32,
						Source = source,
					};
				} else {
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
				Debug.Assert(this.Host.Loaded);
				return this.Index == this.Host.WorkingSpriteIndex;
			}
		}

		public async void uToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			Debug.Assert(this.Host.Loaded);
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

	public class MainSpriteItemController : CustomController {

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
				} else {
					Debug.Assert(this.Host.Loaded);
					return 1.0;
				}
			}
		}

		// ----------------

		public String uTitle_Text {
			get {
				if (this.Index is null) {
					return "";
				} else {
					Debug.Assert(this.Host.Loaded);
					var model = this.Host.Animation.MainSprite!;
					return $"{model.Name}";
				}
			}
		}

		// ----------------

		public String uDescription_Text {
			get {
				if (this.Index is null) {
					return "";
				} else {
					Debug.Assert(this.Host.Loaded);
					var model = this.Host.Animation.MainSprite!;
					return $"{model.FrameRate:F0} - {model.Frame.Count}";
				}
			}
		}

		// ----------------

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
				} else {
					Debug.Assert(this.Host.Loaded);
					return this.Index == this.Host.WorkingSpriteIndex;
				}
			}
		}

		public async void uToggle_OnClick (
			Object          sender,
			RoutedEventArgs args
		) {
			if (sender is not ToggleButton senders) { return; }
			if (this.Index is null) {
			} else {
				Debug.Assert(this.Host.Loaded);
				var lastWorkingSpriteIndex = this.Host.WorkingSpriteIndex;
				var lastWorkingSpriteFrameRate = this.Host.WorkingSpriteFrameRate;
				if (this.Host.Working) {
					await this.Host.UnloadWorkingSprite();
				}
				if (this.Index != lastWorkingSpriteIndex) {
					await this.Host.LoadWorkingSprite(this.Index.Value, null, !this.Host.RemainFrameRate ? null : lastWorkingSpriteFrameRate, null, null);
				}
			}
			return;
		}

		#endregion

	}

}
