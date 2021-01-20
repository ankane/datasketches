require_relative "test_helper"

class FrequentItemsSketchTest < Minitest::Test
  def test_strings
    sketch = DataSketches::FrequentStringsSketch.new(64)
    assert sketch.empty?
    %w(a a b c).each do |v|
      sketch.update(v)
    end

    assert_equal 3, sketch.num_active_items
    assert_in_delta 4, sketch.total_weight
  end
end
