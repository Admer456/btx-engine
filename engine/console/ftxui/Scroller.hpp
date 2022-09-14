
namespace ftxui
{
	class ScrollerBase : public ComponentBase
	{
	public:
		ScrollerBase( Component child ) { Add( child ); }

	private:
		Element Render() final
		{
			auto focused = Focused() ? focus : ftxui::select;
			auto style = Focused() ? inverted : nothing;

			Element background = ComponentBase::Render();
			background->ComputeRequirement();
			size_ = background->requirement().min_y;
			return dbox( {
					   std::move( background ),
					   vbox( {
						   text( L"" ) | size( HEIGHT, EQUAL, selected_ ),
						   text( L"" ) | style | focused,
					   } ),
				} ) |
				vscroll_indicator | yframe | yflex | reflect( box_ );
		}

		bool OnEvent( Event event ) final
		{
			if ( event.is_mouse() && box_.Contain( event.mouse().x, event.mouse().y ) )
				TakeFocus();

			int selected_old = selected_;
			if ( event == Event::ArrowUp || event == Event::Character( 'k' ) ||
				(event.is_mouse() && event.mouse().button == Mouse::WheelUp) ) {
				selected_--;
			}
			if ( (event == Event::ArrowDown || event == Event::Character( 'j' ) ||
				(event.is_mouse() && event.mouse().button == Mouse::WheelDown)) ) {
				selected_++;
			}
			if ( event == Event::PageDown )
				selected_ += box_.y_max - box_.y_min;
			if ( event == Event::PageUp )
				selected_ -= box_.y_max - box_.y_min;
			if ( event == Event::Home )
				selected_ = 0;
			if ( event == Event::End )
				selected_ = size_;

			selected_ = std::max( 0, std::min( size_ - 1, selected_ ) );
			return selected_old != selected_;
		}

		bool Focusable() const final { return false; }

		int selected_ = 0;
		int size_ = 0;
		Box box_;
	};

	Component Scroller( Component child )
	{
		return Make<ScrollerBase>( std::move( child ) );
	}
}  // namespace ftxui
