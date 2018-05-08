../../../pin -t obj-intel64/CS_395T.so -- parsecmgmt -a run -p freqmine -i simsmall &&
mv results.txt ~/CS_395T_data/text_files/freqmine_8hist_full.txt &&
mv reverse_results.txt ~/CS_395T_data/text_files/reverse_freqmine_8hist_full.txt &&


../../../pin -t obj-intel64/CS_395T.so -- parsecmgmt -a run -p swaptions -i simsmall &&
mv results.txt ~/CS_395T_data/text_files/swaptions_8hist_full.txt &&
mv reverse_results.txt ~/CS_395T_data/text_files/reverse_swaptions_8hist_full.txt &&


../../../pin -t obj-intel64/CS_395T.so -- parsecmgmt -a run -p vips -i simsmall &&
mv results.txt ~/CS_395T_data/text_files/vips_8hist_full.txt &&
mv reverse_results.txt ~/CS_395T_data/text_files/reverse_vips_8hist_full.txt &&


../../../pin -t obj-intel64/CS_395T.so -- parsecmgmt -a run -p x264 -i simsmall &&
mv results.txt ~/CS_395T_data/text_files/x264_8hist_full.txt &&
mv reverse_results.txt ~/CS_395T_data/text_files/reverse_x264_8hist_full.txt
